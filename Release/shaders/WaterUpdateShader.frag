#version 330
precision highp float;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

uniform sampler2D texture;
uniform vec2 delta;
in vec2 coord;
out vec4 outputF;
void main() {
	/* get vertex info */
	vec4 info = texture2D(texture, coord);
	
	/* calculate average neighbour height */
	vec2 dx = vec2(delta.x, 0.0);
	vec2 dy = vec2(0.0, delta.y);
	float average = (
		texture2D(texture, coord - dx).r +
		texture2D(texture, coord - dy).r +
		texture2D(texture, coord + dx).r +
		texture2D(texture, coord + dy).r
	) * 0.25;

	/* change the velocity to move toward the average */
	info.g += (average - info.r) * 2.0;

	/* attenuate the velocity a little so waves do not last forever */
	info.g *= 0.995;

	/* move the vertex along the velocity */
	info.r += info.g;

	outputF = info;
}