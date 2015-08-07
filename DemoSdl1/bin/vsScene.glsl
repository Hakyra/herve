#version 330 core

layout(location = 0) in vec3 _vertexPosition_modelspace;
layout(location = 1) in vec3 _vertexNormal_modelspace;
layout(location = 2) in vec2 _vertexUv;

uniform mat4 _P;
uniform mat4 _V;
uniform mat4 _M;
uniform vec3 _lightPosition_worldspace;

out vec3 _normal_cameraspace;
out vec3 _eyeDirection_cameraspace;
out vec3 _lightDirection_cameraspace;
out vec2 _uv;

void main() {

    gl_Position = _P *_V * _M * vec4(_vertexPosition_modelspace, 1.f); 
 	
	_normal_cameraspace = (_V * _M * vec4(_vertexNormal_modelspace, 0.f)).xyz; 
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	vec3 _position_cameraspace = ( _V * _M * vec4(_vertexPosition_modelspace, 1)).xyz;
	_eyeDirection_cameraspace = vec3(0, 0, 0) - _position_cameraspace;
	
	vec3 lightPosition_cameraspace = ( _V * vec4(_lightPosition_worldspace, 1)).xyz;
	_lightDirection_cameraspace = lightPosition_cameraspace - _position_cameraspace;	

    _uv = _vertexUv;
}
