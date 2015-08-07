libHerve

DESCRIPTION:
Static library for developping VR application.
Features: 
    3D rendering (OpenGL, GLSL)
    Wavefront OBJ loader
    image texture (libpng)
    video texture (libvlc)
    3D sound (OpenAL, alure)
    Oculus Rift (linux only)
    animation: spline path
    navigation: walkthrough camera

DEPENDENCIES:
gl glu glew, glm
libpng 
libvlc
openal, alure

BUILD (linux):
make

BUILD (freebsd):  
gmake CXX=clang++

AUTHOR:
Julien Dehos
http://www-lisic.univ-littoral.fr/~dehos