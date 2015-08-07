libHerveOvr

DESCRIPTION:
Oculus rift support for libHerve.
Includes Oculus Rift SDK 0.2.4.

DEPENDENCIES:
libHerve
libudev
xinerama
x11

BUILD (linux):
make
cp 90-oculus.rules /etc/udev/rules.d/

AUTHOR:
Julien Dehos
http://www-lisic.univ-littoral.fr/~dehos
