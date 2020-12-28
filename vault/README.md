# Vault secret server

## Installation and Setup

```shell
docker-compose up --detach --build vault
node bin/dev-set-up.js
```

Vault secrets are saved in `.secrets.json`.  This is insecure an
should only be used on developer VMs.

AppRole credentials are saved in `*-credentials.json`

## Using the API

### Retrieving Vault Tokens from the command line

```shell
curl --data @ipacs-credentials.json\
 http://127.0.0.1:8200/v1/auth/approle/login|jsonlint
curl --data @camunda-credentials.json\
 http://127.0.0.1:8200/v1/auth/approle/login|jsonlint
curl --data @pathology-credentials.json\
 http://127.0.0.1:8200/v1/auth/approle/login|jsonlint
```

The `auth.client_token` value must be set as the `X-Vault-Token`
header for authenticated requests to the vault API.

### Reading and writing secrets

#### Writing secrets

Given a file named `ipacs-camunda.json`, with contents

```json
{
  "data": {
    "ipacs-user": "camunda",
    "ipacs-password": "random string"
  }
}
```

Executing the following will store the data object.

```shell
curl --header "X-Vault-Token: <auth.client_token from ipacs login above>"\
 --data @ipacs-camunda.json\
 http://127.0.0.1:8200/v1/secret/data/ipacs/camunda-credentials|jsonlint
```

#### Reading secrets

```shell
curl --header "X-Vault-Token: <auth.client_token from camunda login above>"\
 http://localhost:8200/v1/secret/data/ipacs/camunda-credentials|jsonlint
```

### Encrypting and decrypting data

#### Encryption

```shell
curl --request POST --header "X-Vault-Token: <auth.client_token from pathology login above>"\
 --data '{"plaintext": "NDExMSAxMTExIDExMTEgMTExMQo="}'\
 http://localhost:8200/v1/pathology/encrypt/viewer|jsonlint
```

Specify base64 encoded plaintext.

```json
{
  "request_id": "57060353-a1c3-43a1-5da1-10289e98d263",
  "lease_id": "",
  "renewable": false,
  "lease_duration": 0,
  "data": {
    "ciphertext": "vault:v1:iwlfJgVgII7X/IHFiKZJMqKDbgPDhe/Pspuuk3T+gL2UIbW3eUIfaCCpNujixEma",
    "key_version": 1
  },
  "wrap_info": null,
  "warnings": null,
  "auth": null
}
```

#### Decryption

```shell
curl --request POST --header "X-Vault-Token: <auth.client_token from pathology login above>"\
 --data '{"ciphertext": "vault:v1:iwlfJgVgII7X/IHFiKZJMqKDbgPDhe/Pspuuk3T+gL2UIbW3eUIfaCCpNujixEma"}'\
 http://localhost:8200/v1/pathology/decrypt/viewer|jsonlint
```

```json
{
  "request_id": "0980b70c-9ef2-c3b4-8448-f24be81dd26b",
  "lease_id": "",
  "renewable": false,
  "lease_duration": 0,
  "data": {
    "plaintext": "NDExMSAxMTExIDExMTEgMTExMQo="
  },
  "wrap_info": null,
  "warnings": null,
  "auth": null
}
```



### Rotate log files

```shell
sudo savelog -n -c 7 logs/audit.log
docker-compose exec vault killall -1 vault
```
