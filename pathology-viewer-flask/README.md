# pathollogy-viewer-flask #
Invicro Pathology Viewer backend with REST API written in Flask and libraries written in C++.


### Build libraries with CMake ###

C++ libraries:
* libs/region_loader
* libs/mld_loader

To build libs go to the root directory for both libraries and perform following:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```


### Install python dependencies ###

Create new environment and install python dependencies with pip.
```
$ pip install <package_name>
```

List of used packages (with current versions):
* autopep8      1.5.4
* click         7.1.2
* cppyy         1.8.0
* cppyy-backend 1.12.1
* cppyy-cling   6.21.0
* CPyCppyy      1.11.1
* Flask         1.1.2
* itsdangerous  1.1.0
* Jinja2        2.11.2
* MarkupSafe    1.1.1
* numpy         1.19.1
* opencv-python 4.3.0.36
* pip           20.1.1
* pycodestyle   2.6.0
* setuptools    41.2.0
* toml          0.10.1
* Werkzeug      1.0.1


### Run Flask API ###

Set flask environment variable to 'development' or 'production' with command (e.g. development):
```
$ export FLASK_ENV=development
```


*This step is not needed if your python application file is named app.py (as in this case).*  
Set flask app variable to the name of your python application file (e.g. test.py):
```
$ export FLASK_APP=test.py
```


Run flask built-in server with command:
```
$ flask run
```
or with command:
```
$ flask run --host=0.0.0.0
```

### Docker build and run

Dockerfile defines a docker image based on ubuntu 20.04 that contains all neccessary packages
to build and run pathollogy-viewer-flask server.
Also, a Makefile is provided to make thing easier. The Makefile contains two targets, one to build
the docker image, and one to run that image as a container.

To build the image:
```
make docker_dev_build_image
```

This may take some time on first run.

To start the backend:
```
make docker_dev_run
```

### Data directory

Data files (mld, svs, etc...) are located to the location defined in DATA_PATH variable in app.py.
Default value is:
DATA_PATH="/data"

Also if there is DATA_PATH set as an OS environment variable, flask will use that location.

When run from docker, inside the image, DATA_PATH os variable is set to /data, and bind to test data directory next to app/.

When running outside the docker you can point the data folder with:
```
DATA_PATH="/my/absolute/path/data" flask run
```

and flask will search for data files into folder /my/absolute/path/data


### Run demo

Flask app hosts frontend for demo purposes.
To access frontend with demo dataset go to:

```
http://localhost:8080/#/1/1/120094
```
