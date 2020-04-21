[![Build Status](https://travis-ci.com/bonewell/spfservice.svg?branch=master)](https://travis-ci.com/bonewell/spfservice)

# spfservice
This project is Microservice of Dijkstra's Shortest Path First algorithm.

## Build
```Shell
$ mkdir build
$ cd build
$ conan install ..
$ cmake ..
$ cmake --build .
```

## Run tests
```Shell
$ ./bin/spfservice_unittest
```

## Run benchmark
```Shell
$ ./bin/spfservice_benchmark
```

## Run service
```Shell
$ ./bin/spfservice
```