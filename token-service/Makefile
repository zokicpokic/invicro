# Build DockerDev Image
build_image:
	docker build --tag token-service:latest --file Dockerfile .

# Run DockerDev Image
dev_run:
	docker run --rm -it\
		--env TOKEN_DURATION=700\
		--name tokenService\
		--net=host\
		token-service:latest
