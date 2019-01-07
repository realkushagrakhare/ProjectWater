#version 330
precision highp float;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

const float PI = 3.141592653589793;

uniform sampler2D texture;
uniform vec2 center;
uniform float radius;
uniform float strength;
in vec2 coord;
out vec4 outputF;
void main() {
	/* get vertex info */
	vec4 info = texture2D(texture, coord);

	/* add the drop to the height */
	float drop = max(0.0, 1.0 - length(center * 0.5 + 0.5 - coord) / radius);
	drop = 0.5 - cos(drop * PI) * 0.5;
	info.r += drop * strength;

	outputF = info;
}
