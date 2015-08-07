#version 330 core

layout(location = 0) out vec4 _fragmentColor;

in vec2 _uv;
in vec3 _position_worldspace;
in vec3 _normal_worldspace;

uniform sampler2D _texture;
uniform	vec3 _ambientColor;
uniform	vec3 _diffuseColor;
uniform	vec3 _lightColor;
uniform vec3 _lightPosition_worldspace;

uniform int _useUv;
uniform int _useTexture;
uniform vec2 _textureSize;

void main() {

	// diffuse color 
	vec3 diffuseColor = _diffuseColor;
	if (_useTexture == 1) {
		vec2 uv;
		if (_useUv == 1) uv = _uv;
		else uv = (gl_FragCoord.xy)/_textureSize;
		diffuseColor *= texture( _texture, uv).rgb;
	}
	
	// Normal of the computed fragment, in camera space
	vec3 n = normalize( _normal_worldspace );
	
	// Direction of the light (from the fragment to the light)
	vec3 l0 = _lightPosition_worldspace - _position_worldspace;
	vec3 l = normalize( l0 );
	float distance = length(l0);
	
	// Cosine of the angle between the normal and the light direction
	float cosTheta = clamp( dot(n, l), 0, 1 );
		
	// tone mapping (ugly)
	_fragmentColor.rgb = _ambientColor + diffuseColor * clamp(log(1+ cosTheta * _lightColor / (distance)), 0, 2);
	_fragmentColor.a = 1;
}
