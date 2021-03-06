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

## Json API
### Add vertex
#### Request
```Json
{"action": "AddVertex"}
```

#### Response
```Json
{"id": "<Number>"}
```

### Remove vertex
#### Request
```Json
{"action": "RemoveVertex", "id": <Number>}
```

#### Response
```Json
{}
```
_Note: all edges of the vertex are removing._

### Add edge or update weight
#### Request
```Json
{"action": "AddEdge", "from": <Number>, "to": <Number>, "weight": <Number>}
```

#### Response
```Json
{}
```

### Remove edge
#### Request
```Json
{"action": "RemoveEdge", "from": <Number>, "to": <Number>}
```

#### Response
```Json
{}
```

### Get path
#### Request
```Json
{"action": "GetPath", "from": <Number>, "to": <Number>}
```

#### Response
```Json
{"ids": ["<Number>", ...]}
```

### Error response
```Json
{"error": "<String>"}
```