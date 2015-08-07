# Demo

## Description
This is a skeleton application for developping a VR application with the herve library. 
This demo creates a SDL window, checks for oculus rift HMD and displays a very basic scene.

## Dependencies
SDL1
libHerve
libHerveOvr
...

## Build (linux):
build libHerve and libHerveOvr 
``` 
make 
```

Note for ubuntu user : 
if you get this error: Inconsistency detected by ld.so: ...
then try: 
```
make clean; make LDFLAGS=-L/usr/lib/nvidia-304/ # check version
```

## Run:
```
cd bin
./Demo.out
```
