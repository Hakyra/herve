#version 330 core

layout(location = 0) out vec3 _fragmentColor;

in vec3 _normal_cameraspace;
in vec3 _eyeDirection_cameraspace;
in vec3 _lightDirection_cameraspace;
in vec2 _uv;

uniform	vec3 _ambientColor;
uniform	vec3 _diffuseColor;
uniform	vec3 _specularColor;
uniform	float _specularIndex;
uniform	vec3 _lightColor;
uniform sampler2D _texture;
uniform int _useTexture;

void main() {
	
	vec3 n = normalize( _normal_cameraspace );
	float distance = length(_lightDirection_cameraspace);
	vec3 l = normalize(_lightDirection_cameraspace );
	vec3 i = normalize(_eyeDirection_cameraspace);
	vec3 r = reflect(-i, n);
	
	float cosTheta = clamp( dot(n, l), 0, 1 );
	float cosAlpha = clamp( dot(l, r), 0, 1 );
	
	// diffuse color 
	vec3 diffuse =  _diffuseColor * cosTheta;
	if (_useTexture != 0) diffuse *= texture( _texture, _uv).rgb;
	
	vec3 specular = _specularColor * pow(cosAlpha, _specularIndex);
		
	_fragmentColor = clamp(_ambientColor + (diffuse+specular)*_lightColor / distance, 0, 1);
}
