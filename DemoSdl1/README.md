DemoSdl1

DESCRIPTION:
Demo for developping VR application with SDL1 + herve.
3D rendering, 3D sound, video texture...
Oculus rift, gamepad (tested with Logitech Gamepad F310)

DEPENDENCIES:
sdl1
libHerve
libHerveOvr
openal
libvlc
...

BUILD (linux):
build libHerve and libHerveOvr
make

NOTE FOR UBUNTU USER : 
if you get this error: Inconsistency detected by ld.so: ...
then try: make clean; make LDFLAGS=-L/usr/lib/nvidia-304/ # check version

RUN:
cd bin
./DemoSdl1.out

AUTHOR:
Julien Dehos
http://www-lisic.univ-littoral.fr/~dehos
