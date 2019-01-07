#version 330
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

in vec4 Vertex;

vec4 ftransform() {
	return ModelViewProjectionMatrix * Vertex;
}

uniform sampler2D water;
out vec3 position;

void main() {
	vec4 info = texture2D(water, Vertex.xy * 0.5 + 0.5);
	position = Vertex.xzy;
	position.y += info.r;
	gl_Position = ModelViewProjectionMatrix * vec4(position, 1.0);
}
