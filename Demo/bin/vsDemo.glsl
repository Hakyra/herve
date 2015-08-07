#version 330 core

layout(location = 0) in vec3 _vertexPosition_modelspace;
layout(location = 1) in vec3 _vertexNormal_modelspace;
layout(location = 2) in vec2 _vertexUv;

out vec2 _uv;

void main() {

    gl_Position = vec4(_vertexPosition_modelspace, 1.0); 
 	
    _uv = _vertexUv;
}
