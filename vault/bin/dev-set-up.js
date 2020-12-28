const util = require('util')
const fs = require('fs')
const readFile = util.promisify(fs.readFile)
const writeFile = util.promisify(fs.writeFile)

const { getJSON, putJSON, postJSON } = require('../lib/json-http')

const isInitialized = async base => {
  return getJSON(`${base}/sys/init`)
    .then(initialized => initialized.initialized)
}

const getSealStatus = async base => {
  return getJSON(`${base}/sys/seal-status`)
}

const initialize = async (base, t, n) => {
  return putJSON(`${base}/sys/init`, {
    pojo: {
      secret_shares: n,
      secret_threshold: t
    }
  })
}

const writeSecrets = async (filename, secrets) => {
  return writeFile(filename, JSON.stringify(secrets))
}

const unsealStep = async (base, key) => {
  return putJSON(`${base}/sys/unseal`, {
    pojo: {
      key
    }
  })
}

// TODO - handle partially unsealed case
// need sealStatus.progress and `await unsealStep`
const unseal = async (base, sealStatus, secrets) => {
  if (secrets.keys.length < sealStatus.t) throw new Error('Too few keys to unseal')
  const unsealCalls = []
  for (let i = 0; i < sealStatus.t; i++) {
    unsealCalls.push(unsealStep(base, secrets.keys[i]))
  }
  return Promise.all(unsealCalls)
}

const isEnabled = async (url, token, name, type) => {
  const enabled = await getJSON(url, { token })
  return enabled.data[`${name}/`]?.type === type
}

const isAuditLoggingEnabled = async (base, token, logName) => {
  return isEnabled(`${base}/sys/audit`, token, logName, 'file')
}

const enableAuditLogging = async (base, token, logName) => {
  return putJSON(`${base}/sys/audit/${logName}`, {
    token,
    pojo: {
      type: 'file',
      options: {
        file_path: '/vault/logs/audit.log'
      }
    }
  })
}

const readSecrets = async filename => {
  return readFile(filename, 'utf-8')
    .then(JSON.parse)
}

// TODO verify accessor .startsWith('kv_')
// throw on name/accessor mismatch
const isKeyValueEnabled = async (base, token, name) => {
  return isEnabled(`${base}/sys/mounts`, token, name, 'kv')
}

const enableKeyValue = async (base, token, name, maxVersions) => {
  // on success, 204 No Content
  return postJSON(`${base}/sys/mounts/${name}`, {
    token,
    pojo: {
      type: 'kv',
      description: 'key/value secret store',
      options: {
        version: 2
      }
    }
  }) // on success, 204 No Content
    .then(() => postJSON(`${base}/${name}/config`, {
      token,
      pojo: {
        max_versions: maxVersions
      }
    }))
}

// TODO verify accessor .startsWith('auth_approle_')
// throw on name/accessor mismatch
const isAppRoleEnabled = async (base, token, name) => {
  return isEnabled(`${base}/sys/auth`, token, name, 'approle')
}

const enableAppRole = async (base, token, name) => {
  // on success, 204 No Content
  return postJSON(`${base}/sys/auth/${name}`, {
    token,
    pojo: {
      type: 'approle',
      description: 'approle authentication'
    }
  })
}

const isTransitEnabled = async (base, token, name) => {
  return isEnabled(`${base}/sys/mounts`, token, name, 'transit')
}

const enableTransit = async (base, token, name) => {
  // on success, 204 No Content
  return postJSON(`${base}/sys/mounts/${name}`, {
    token,
    pojo: {
      type: 'transit',
      description: 'encryption as a service'
    }
  })
}

const writeCredentials = async (filename, credentials) => {
  return writeFile(filename, JSON.stringify(credentials))
}

// TODO
// - check for policy

const writePolicy = async (base, token, name, policy) => {
  return putJSON(`${base}/sys/policy/${name}`, {
    token,
    pojo: {
      policy: JSON.stringify(policy)
    }
  })
}

const writeKeysAndValues = (base, token, name, path, kvs) => {
  return postJSON(`${base}/${name}/data/${path}`, {
    token,
    pojo: {
      data: kvs
    }
  })
}

const createAppRole = async (base, token, name, policies) => {
  return postJSON(`${base}/auth/approle/role/${name}`, {
    token,
    pojo: {
      policies: policies.join(',')
    }
  }).then(() => {
    return Promise.all([
      getJSON(`${base}/auth/approle/role/${name}/role-id`, { token }),
      postJSON(`${base}/auth/approle/role/${name}/secret-id`, { token })
    ])
  }).then(roleData => {
    const [role, secret] = roleData
    return writeCredentials(`./${name}-credentials.json`, {
      role_id: role.data.role_id,
      secret_id: secret.data.secret_id
    })
  })
}

const setUpVault = async (base) => {
  let secrets
  if (!await isInitialized(base)) {
    console.log('Initializing Vault and saving secrets')
    secrets = await initialize(base, 3, 5)
    await writeSecrets('./.secrets.json', secrets)
  } else {
    console.log('Reading vault secrets')
    secrets = await readSecrets('./.secrets.json')
  }
  const token = secrets.root_token
  let sealStatus = await getSealStatus(base)
  if (sealStatus.sealed) {
    console.log('Unsealing Vault')
    sealStatus = await unseal(base, sealStatus, secrets)
  }
  if (!await isAuditLoggingEnabled(base, token, 'audit-logs')) {
    console.log('Enabling audit logs')
    await enableAuditLogging(base, token, 'audit-logs')
  }
  if (!await isKeyValueEnabled(base, token, 'secret')) {
    console.log('Enabling Key/Value store')
    await enableKeyValue(base, token, 'secret', 5)
  }
  await writePolicy(base, token, 'ipacs-admin', {
    path: [{
      'secret/data/ipacs/*': {
        capabilities: [
          'create',
          'read',
          'update',
          'delete',
          'list'
        ]
      }
    }]
  })
  await writePolicy(base, token, 'ipacs-login', {
    path: [{
      'secret/data/ipacs/camunda-*': {
        capabilities: [
          'read'
        ]
      }
    }]
  })
  if (!await isTransitEnabled(base, token, 'pathology')) {
    console.log('Enabling encryption as a service')
    await enableTransit(base, token, 'pathology')
  }
  await writePolicy(base, token, 'pathology-encryption', {
    path: [{
      'pathology/encrypt/viewer': {
        capabilities: [
          'update',
          'create'
        ]
      }
    }, {
      'pathology/decrypt/viewer': {
        capabilities: [
          'update',
          'create'
        ]
      }
    }]
  })
  await writeKeysAndValues(base, token, 'secret', 'ipacs/example-keys-values', {
    example: 'keys and values',
    key: 'value'
  })
  if (!await isAppRoleEnabled(base, token, 'approle')) {
    console.log('Enabling AppRole')
    await enableAppRole(base, token, 'approle')
  }
  await createAppRole(base, token, 'ipacs', [
    'ipacs-admin'
  ])
  await createAppRole(base, token, 'camunda', [
    'ipacs-login'
  ])
  await createAppRole(base, token, 'pathology', [
    'pathology-encryption'
  ])
}

const BASE_URL = 'http://localhost:8200/v1'

setUpVault(BASE_URL)
  .then(() => console.log('Vault ready'))
  .catch(console.error)
