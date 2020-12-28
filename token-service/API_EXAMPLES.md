### Start vault:

note:
error in bin/dev-set-up.js scripts.

line 231 and 237, capabilities array contains only 'update' value. Add 'create' value:

      'pathology/encrypt/viewer': {
        capabilities: [
          'update',
          'create'
        ]
      }


call docker-compose (docker-compose must be installed):
```
docker-compose up --detach --build vault
```

after that, execute node command (node must be isntalled):
```
node bin/dev-set-up.js
```
File `pathology-credentials.json` will be created.

Yea!

### 1. Auth postman/curl to Vault

```
curl --data @pathology-credentials.json http://127.0.0.1:8200/v1/auth/approle/login
```

Returned data:
```
{
   "auth" : {
      "accessor" : "A8iyn4hIjKQsscIPF2HdFOwL",
      "client_token" : "s.YdyMW6nLZ2oN7ZRM64UMKOfJ",
      "entity_id" : "ff83d9d7-1251-33e1-4b85-9e42b0bb086a",
      "lease_duration" : 604800,
      "metadata" : {
         "role_name" : "pathology"
      },
      "orphan" : true,
      "policies" : [
         "default",
         "pathology-encryption"
      ],
      "renewable" : true,
      "token_policies" : [
         "default",
         "pathology-encryption"
      ],
      "token_type" : "service"
   },
   "data" : null,
   "lease_duration" : 0,
   "lease_id" : "",
   "renewable" : false,
   "request_id" : "928c4dd8-1f29-bd88-092c-d1111bb184e3",
   "warnings" : null,
   "wrap_info" : null
}
```

"client_token" is what we need! (on your computer the value will be different)

### 2. Create token postman/curl to Token Service

```
curl -X POST --data 'auth_token=s.YdyMW6nLZ2oN7ZRM64UMKOfJ' http://127.0.0.1:5001/create
```

Returned data:

```
{
  "token": "dmF1bHQ6djE6SUZhZit3MlZscklUQVBTbEMxdXJJc1FLYlI5blJMNTA0U1BUaG12aDRoVlVCQkxrbE93Tk0xaDEzaVdPYmRzeFMrUVZ1cnJkZnNnbFdMZ2lab2QweC83MkptWk5mTUJFcWx1NEl3MW43WjNEKzJCZVRGdHViQlNydHplYQ=="
}
```

This value should be sent to url as ?token=dmF1...&auth_token=s.YdyMW6nLZ2oN7ZRM64UMKOfJ


### 3. Validate token (frontend) to Token Service


```
curl -X GET 'http://127.0.0.1:5001/validate?token=dmF1bHQ6djE6VGJVaFg0U21rZ0FYV0hHNzFtSGZSM3VlSnZsc0JORUZEd3ZzZW5RMEF1aDIrSDJVcER5NFV4RUVIb0RpYk15aXhza2gzQlVsOHJ1c0N5emRyL3ArR3lnSVU1aE9oZmI2d1VvUWR5VUswbElTWFRrPQ==&auth_token=s.YdyMW6nLZ2oN7ZRM64UMKOfJ'
```

If everything is ok, the returned data will be

```
{
  "exp_time": 1609017597.8422377, 
  "some_data": "abc123"
}
```
