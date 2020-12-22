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


