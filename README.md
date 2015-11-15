# Luna
Very tiny SW Platform for Web API using C++ and mongoose web server.

For building TestWebServer...

[Linux]
$ cd Luna/Library/ccCoreAPI/build
$ make -f makefile.linux.i386.debug.txt all

$ cd Luna/Library/ccWebServerAPI/build
$ make -f makefile.linux.i386.debug.txt all

$ cd Luna/ThirdPartyLibrary/ccMongooseServerAPI/build
$ make -f makefile.linux.i386.debug.txt all

$ cd Luna/TestAPPs/WebServerTest/build
$ make -f makefile.linux.i386.debug.txt all
$ ./linux.i386.debug/WebServerTest

[OS X]
$ cd Luna/Library/ccCoreAPI/build
$ make -f makefile.osx.i386.debug.txt all

$ cd Luna/Library/ccWebServerAPI/build
$ make -f makefile.osx.i386.debug.txt all

$ cd Luna/ThirdPartyLibrary/ccMongooseServerAPI/build
$ make -f makefile.osx.i386.debug.txt all

$ cd Luna/TestAPPs/WebServerTest/build
$ make -f makefile.osx.i386.debug.txt all
$ ./osx-i386-all-debug/WebServerTest
