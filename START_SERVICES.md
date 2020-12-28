### How to start all services

## Requirements

Needed software:
docker-compose
node

## Prepare images

Build token service image and pathology-viewer-flask

```
cd token-service
make build_image
```

and

```
cd pathology-viewer-flask
make docker_dev_build_image
```

Vault already has pre-made image on docker-hub (called vault).

## Run docker services

The Main docker-compose file is located in pathology-viewer-flask.
Take a look. It contains the definition of all three services (vault, pathology and token).
Check environment variable and edit if needed:
```
TOKEN_DURATION:600
```

Start all services with (from pathology-viewer-flask directory):
```
docker-compose up
```

all services should start and you should see the output of all three services in the console.
Leave that terminal.
Open another one and go to the vault directory
```
cd vault
```

We need to initialize vault:
```
node bin/dev-set-up.js
```
(NOTE: every time you (re)start vault service this initialization step must be called.

Now we have all services up and running.


## Scripts

Makefile in the root of the repo containe targets for every endpoint.
Install on your linux the following packages (if they are not already installed):
json_pp
grep
awk
sed

To get vault_token:
```
make vault_token
```

To create token on token service:
```
make create_token
```

To generate url params that needs to be passed to the frontend:
```
make frontend_url_params
```

To generate url for validation:
```
make validate_url
```

And to call validation endpoint:
```
make call_validation
```


