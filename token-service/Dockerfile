FROM ubuntu:20.04
LABEL version="1.0.0" \
    description="Token service"

# Update package manager
RUN apt -y update

# Install required packages
RUN apt-get -y install python python3-pip

# Clean packages installer
RUN rm -rf /var/lib/apt/lists/*

# Install python3 modules
COPY app/requirements.txt /app/
RUN pip3 install -r /app/requirements.txt

# Copy required files to container
COPY app /app

# Setup environement
# set TOKEN_DURATION in seconds
WORKDIR /app
ENV LC_ALL=C.UTF-8 \
    LANG=C.UTF-8 \
    FLASK_APP=/app/app.py \
    FLASK_ENV=development \
    TOKEN_DURATION=600 \
    FLASK_RUN_PORT=5001

# Start flask
RUN chmod +x /app/app.py
CMD [ "python3", "/app/app.py" ]
