# Token service

### Installing dependencies

```
pip install -r requirements.txt
```

### Runnint the service

Run on port 5001
```
FLASK_RUN_PORT=5001 flask run
```

```
python app.py
```

### Build docker image

Edit Dockerfile if needed:
FLASK_RUN_PORT=<PORT_NUMBER>  # default 5001

```
make build_image
```

### Run docker container

```
make dev_run
```

### Token duration

Token duration is read from env variable TOKEN_DURATION.
If TOKEN_DURATION is not set, token service falls bakc to defatul value 600s (10minutes).

When running as python/flask app token service can be set in similar way as FLASK_RUN_PORT:
```
TOKEN_DURATION=700 FLASK_RUN_PORT=5001 flaks run
```

Also TOKEN_DURATION default value may be changed during the docker image build process (change the value in Dockerfile, and rebuild the image.

And, finally, most elegant way (it doesn't require rebuilding of the code, or the docker image) is to pass TOKEN_DURATION to docker run command: 

```
	docker run --rm -it\
		--env TOKEN_DURATION=700\
		--name tokenService\
		--net=host\
		token-service:latest
```

or simply edit 'dev_run' target in Makefile and change TOKEN_DURATION value.
