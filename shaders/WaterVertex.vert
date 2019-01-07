#version 330
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

in vec4 Vertex;

vec4 ftransform() {
	return ModelViewProjectionMatrix * Vertex;
}

out vec2 coord;
void main() {
	coord = Vertex.xy * 0.5 + 0.5;
	gl_Position = vec4(Vertex.xyz, 1.0);
}
