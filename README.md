# Luna
Very tiny SW Platform for Web API using C++ and mongoose web server.

## How to build
### step1. make dependency libraries
### step2. make Luna
### step3. make applications

## Build
## STEP1: How to build dependency libraries
There are nothings to build dependency libraries.
If you don't need to use cURL, Mbedtls or MQTT, you have to turn off options for their in CMakeLists.txt.

## STEP2: How to build Luna
### Linux & OS X
```bash
cd ~/Luna
mkdir build  
cd build
cmake ..
make
```

## STEP3: How to build a test application to test Web Server and Websocket
### Linux
```bash
cd ./Luna/build
./bin/WebServerTest
```
