const { request } = require('http')

// TODO - DRY request functions
// - refactor common code into a child function
// - handle 204 for PUT, POST
// - improve reject errors for programatic handling

const getJSON = async (url, { token } = {}) => {
  return new Promise((resolve, reject) => {
    const headers = {
      Accept: 'application/json'
    }
    if (token) headers['X-Vault-Token'] = token
    const req = request(url, {
      headers
    }, res => {
      const { statusCode, headers } = res
      if (!/^application\/json/.test(headers['content-type'])) {
        return reject(new Error(`getJSON failed with 'content-type': ${headers['content-type']}`))
      }
      let json = ''
      res.setEncoding('utf8')
      res.on('data', data => (json += data))
      res.on('end', () => {
        const body = JSON.parse(json)
        if (statusCode === 200) resolve(body)
        else {
          reject(new Error(JSON.stringify({
            statusCode,
            headers,
            body
          })))
        }
      })
    })
    req.on('error', reject)
    req.end()
  })
}

const putJSON = async (url, { token, pojo }) => {
  return new Promise((resolve, reject) => {
    const json = JSON.stringify(pojo)
    const headers = {
      Accept: 'application/json',
      'Content-Type': 'application/json',
      'Content-Length': json.length
    }
    if (token) headers['X-Vault-Token'] = token
    const req = request(url, {
      method: 'PUT',
      headers
    }, res => {
      const { statusCode, headers } = res
      if (!/^application\/json/.test(headers['content-type'])) {
        return reject(new Error(`putJSON failed with 'content-type': ${headers['content-type']}`))
      }
      let json = ''
      res.setEncoding('utf8')
      res.on('data', data => (json += data))
      res.on('end', () => {
        if (statusCode === 200) resolve(JSON.parse(json))
        else if (statusCode === 204) resolve(null)
        else {
          reject(new Error(JSON.stringify({
            statusCode,
            headers,
            body: JSON.parse(json)
          })))
        }
      })
    })
    req.on('error', reject)
    req.write(json)
    req.end()
  })
}

const postJSON = async (url, { token, pojo }) => {
  return new Promise((resolve, reject) => {
    const json = JSON.stringify(pojo)
    const headers = {
      Accept: 'application/json',
      'Content-Type': 'application/json',
      'Content-Length': pojo ? json.length : 0
    }
    if (token) headers['X-Vault-Token'] = token
    const req = request(url, {
      method: 'POST',
      headers
    }, res => {
      const { statusCode, headers } = res
      if (!/^application\/json/.test(headers['content-type'])) {
        return reject(new Error(`postJSON failed with 'content-type': ${headers['content-type']}`))
      }
      let json = ''
      res.setEncoding('utf8')
      res.on('data', data => (json += data))
      res.on('end', () => {
        if (statusCode === 200) resolve(JSON.parse(json))
        else if (statusCode === 204) resolve(null)
        else {
          reject(new Error(JSON.stringify({
            statusCode,
            headers,
            body: JSON.parse(json)
          })))
        }
      })
    })
    req.on('error', reject)
    if (pojo) req.write(json)
    req.end()
  })
}

module.exports = {
  getJSON,
  putJSON,
  postJSON
}
