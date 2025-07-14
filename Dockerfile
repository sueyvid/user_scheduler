FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y \
    build-essential \
    make \
    vim \
    gdb \
    git \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . /app
CMD ["/bin/bash"]
