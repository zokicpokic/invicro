FROM ubuntu:20.04
LABEL version="1.0.0" \
    description="Pathology Viewer Server container"

# Update package manager
RUN apt -y update

# Install required packages
RUN apt-get -y install openslide-tools
RUN apt-get -y install build-essential cmake \
    libgl-dev libtbb2 liblapack-dev libatlas-base-dev \
    python python3-pip

# Clean packages installer
RUN rm -rf /var/lib/apt/lists/*

# Install python3 modules
COPY app/requirements.txt /app/
RUN pip3 install scikit-build && pip3 install -r /app/requirements.txt

# Copy required files to container
COPY app /app
COPY setup /setup/

# Build mld loader
# RUN chmod +x ../setup/build_mld.sh && sh ../setup/build_mld.sh

# Build region loader
# RUN chmod +x ../setup/build_loader.sh && sh ../setup/build_loader.sh

WORKDIR /app/libs/mld_loader/build
RUN ["cmake", ".."]
RUN ["make"]

# build region loader
WORKDIR /app/libs/region_loader/build
RUN ["cmake", ".."]
RUN ["make"]


# Setup environement 
WORKDIR /app
ENV LC_ALL=C.UTF-8 \
    LANG=C.UTF-8 \
    FLASK_APP=/app/app.py \
    FLASK_ENV=development \
    DATA_PATH=/data

# Start flask
RUN chmod +x /app/app.py
CMD [ "python3", "/app/app.py" ]
