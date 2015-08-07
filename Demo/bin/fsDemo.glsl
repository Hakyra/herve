#version 330 core

layout(location = 0) out vec3 _fragmentColor;

in vec2 _uv;

void main() {
			
	_fragmentColor = vec3( _uv.x, _uv.y, 0.0);
}
