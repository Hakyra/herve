# DemoGtkmm

## DESCRIPTION
This demo is just a proof-of-concept for developping a VR application + control window with the herve library.

## DEPENDENCIES
gtkmm
gtkglextmm
libHerve
...

## BUILD (linux):
build libHerve 
```
make
```

NOTE FOR UBUNTU USER : 
if you get this error: Inconsistency detected by ld.so: ...
then try: 
```
make clean; make LDFLAGS=-L/usr/lib/nvidia-304/ # check version
```

## BUILD (freebsd):  
```
build libHerve
gmake CXX=clang++
```

## RUN:
```
cd bin
./DemoGtkmm.out
```
