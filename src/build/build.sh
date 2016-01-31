#!/bin/bash

BASE=`pwd`
USERNAME=`whoami`
EXT=`date +%k.%M.%S`
LOG=0
FINDS=""
LIB_PATH=""
POWER=""
LIB_DIRS="Library FossLibrary ProductLibrary ThirdPartyLibrary"
SCOPE=""
OPTION="all"
TARGET_OS=""

showlog()
{
  echo "ex) build.sh [linux/osx][all/clean]"
  echo "ex) oneshot.sh linux all"
  echo "ex) oneshot.sh osx clean"
}

SCOPE="Library"

case $1 in
  "linux")
        TARGET_OS="linux"
        CPU="i386"
        PRODUCT="luna"
        ;;
  "osx") 
        TARGET_OS="osx"
        CPU="i386"
        PRODUCT="luna"
        ;;
  *)
        echo "input os type....."
        showlog
        exit 1
        ;;
esac

case $2 in
  "all") 
        OPTION="all"
        ;;
  "clean")
        OPTION="clean"
        ;;
  *)
        OPTION="all"
        ;;
esac

POWER="-j5"
FILE="makefile."$TARGET_OS"."$CPU".debug.txt"

make_lib()
{
  for TMP in $LIB_DIRS
  do
    echo "Starting make lib"

    for SUB in $TMP 
    do
      FINDS=`find ../$SUB -name $FILE -exec dirname {} \;`

      for DIR in $FINDS
      do
        cd $DIR        
        make -f $FILE $OPTION
        ERR=$?        
        if [ $ERR -ne 0 ] 
        then
          echo "Make Failed........... at $PWD"
          echo ""
          cd $BASE
          exit 1
        fi
        cd $BASE
      done
    done
    
  #   echo "Library/$TMP/$SUBS"
  done
}

main()
{
  echo "Starting main......"

  make_lib
}

main

FINISH_TIME=`date +%k.%M.%S`

echo "++++++++++++ MAKE REPORT ++++++++++++"
echo "START TIME: $EXT"
echo "END   TIME: $FINISH_TIME"
echo ""
echo "............. $SECONDS second(s) spent...."
