# Luna
Very tiny SW Platform for Web API using C++ and mongoose web server.

For building TestWebServer...

## Linux
### ccCoreAPI
  $ cd Luna/src/Library/ccCoreAPI/build

  $ make -f makefile.linux.i386.debug.txt all

### ccWebServerAPI
  $ cd Luna/src/Library/ccWebServerAPI/build
  
  $ make -f makefile.linux.i386.debug.txt all

### ccMongooseServerAPI
  $ cd Luna/src/ThirdPartyLibrary/ccMongooseServerAPI/build
  
  $ make -f makefile.linux.i386.debug.txt all

### WebServerTest
  $ cd Luna/src/TestAPPs/WebServerTest/build
  
  $ make -f makefile.linux.i386.debug.txt all

### Run
  $ ./linux.i386.debug/WebServerTest

## OS X
### ccCoreAPI
  $ cd Luna/src/Library/ccCoreAPI/build
  
  $ make -f makefile.osx.i386.debug.txt all

### ccWebServerAPI
  $ cd Luna/src/Library/ccWebServerAPI/build
  
  $ make -f makefile.osx.i386.debug.txt all

### ccMongooseServerAPI
  $ cd Luna/src/ThirdPartyLibrary/ccMongooseServerAPI/build
  
  $ make -f makefile.osx.i386.debug.txt all

### WebServerTest
  $ cd Luna/src/TestAPPs/WebServerTest/build
  
  $ make -f makefile.osx.i386.debug.txt all
  
### Run
  $ ./osx-i386-all-debug/WebServerTest
