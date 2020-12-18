#!/usr/bin/env bash

## Check Docker installation
if [[ $(which docker) ]]; then
  DOCKER_COMPOSE_COMMAND="docker-compose";
elif [[ $(which docker-compose.exe) ]]; then
  DOCKER_COMPOSE_COMMAND="docker-compose.exe";
else
  printf "Could not find a docker installation."
  exit 1
fi

PROJECT_NAME=pathology-viewer
YAMLFILE=docker-compose.yml
OPTS="-f $YAMLFILE -p $PROJECT_NAME"
UPOPTS="-d --no-recreate --no-build --no-deps"

case "$1" in
    start)
        echo -n "Starting Pathology Viewer Services..."
        $DOCKER_COMPOSE_COMMAND $OPTS up $UPOPTS
        ;;
    stop)
        echo -n "Stopping Pathology Viewer Services..."
        $DOCKER_COMPOSE_COMMAND $OPTS stop
        $DOCKER_COMPOSE_COMMAND $OPTS down
        ;;
    update)
        echo -n "Updating Pathology Viewer Services..."
        $DOCKER_COMPOSE_COMMAND $OPTS stop
        $DOCKER_COMPOSE_COMMAND $OPTS down
        $DOCKER_COMPOSE_COMMAND $OPTS pull
        $DOCKER_COMPOSE_COMMAND $OPTS up $UPOPTS
        ;;
    restart)
        echo -n "Restarting Pathology Viewer Services..."
	$DOCKER_COMPOSE_COMMAND $OPTS stop
	$DOCKER_COMPOSE_COMMAND $OPTS down
	$DOCKER_COMPOSE_COMMAND $OPTS up
        ;;
    *)
        echo -n "Usage: /etc/init.d/dockercompose {start|stop|restart|update}"
        exit 1
        ;;
esac

exit 1
