DemoGlut

DESCRIPTION:
Demo for developping VR application with GLUT + herve.
3D rendering, 3D sound, video texture...

DEPENDENCIES:
glut
libHerve
openal
libvlc
...

BUILD (linux):
build libHerve 
make

NOTE FOR UBUNTU USER : 
if you get this error: Inconsistency detected by ld.so: ...
then try: make clean; make LDFLAGS=-L/usr/lib/nvidia-304/ # check version

BUILD (freebsd):  
build libHerve 
gmake CXX=clang++

RUN:
cd bin
./DemoGlut.out

AUTHOR:
Julien Dehos
http://www-lisic.univ-littoral.fr/~dehos
