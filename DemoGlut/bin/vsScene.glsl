#version 330 core

layout(location = 0) in vec3 _vertexPosition_modelspace;
layout(location = 1) in vec3 _vertexNormal_modelspace;
layout(location = 2) in vec2 _vertexUv_modelspace;

uniform mat4 _VP;
uniform mat4 _M;

out vec2 _uv;
out vec3 _position_worldspace;
out vec3 _normal_worldspace;

void main() {

    gl_Position = _VP * _M * vec4(_vertexPosition_modelspace, 1.f); 
    
	// UV of the vertex.
    _uv = _vertexUv_modelspace;
    
	// Position of the vertex, in worldspace : M * position
	_position_worldspace = (_M * vec4(_vertexPosition_modelspace, 1.f)).xyz;
	
	// Normal of the the vertex, in camera space
	_normal_worldspace = ( _M * vec4(_vertexNormal_modelspace, 0.f)).xyz; 
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
