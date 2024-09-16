FROM ubuntu:23.04

WORKDIR /app

RUN apt-get update && apt-get install -y \
    clang-16 \
    libboost-all-dev \
    libpq-dev \
    postgresql-15 \
    wget \
    git \
    cmake \
    make

COPY . /app

ENV BOOST_ROOT=/usr/include/boost
ENV PATH=$PATH:/usr/bin/clang-17
ENV CC=clang-16
ENV CXX=clang++-16
ENV CMAKE=cmake
ENV MAKE=make

RUN mkdir -p cmake
RUN wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake

RUN cmake -B build --fresh
RUN cmake --build build

CMD ["./build/Server"]