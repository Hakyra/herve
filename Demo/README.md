Demo

DESCRIPTION
This is a skeleton application for developping a VR application with the herve library. 
This demo creates a SDL window, checks for oculus rift HMD and displays a very basic scene.

DEPENDENCIES
SDL1
libHerve
libHerveOvr
...

BUILD (linux):
build libHerve and libHerveOvr 
make

NOTE FOR UBUNTU USER : 
if you get this error: Inconsistency detected by ld.so: ...
then try: make clean; make LDFLAGS=-L/usr/lib/nvidia-304/ # check version

RUN:
cd bin
./Demo.out

AUTHOR:
Julien Dehos
http://www-lisic.univ-littoral.fr/~dehos
