FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    pkg-config \
    libjsoncpp-dev \
    libsodium-dev \
    libfmt-dev \
    libspdlog-dev \
    libmysqlcppconn-dev \
    python3 \
    python3-pip \
    python3-matplotlib \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

CMD ["./app"]