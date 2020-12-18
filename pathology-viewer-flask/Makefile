# Build DockerDev Image
docker_dev_build_image:
	docker build --tag pathology-server:latest --file Dockerfile .

# Run DockerDev Image
docker_dev_run:
	docker run --rm -it\
		--name pathologyBackend\
		--net=host\
		--mount type=bind,src=`pwd`/../data,dst=/data\
		--mount type=bind,src=`pwd`/../config/server.json,dst=/app/static/configuration.json\
		pathology-server:latest
