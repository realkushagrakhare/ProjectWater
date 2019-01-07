#version 330
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

in vec4 Vertex;
in vec4 TexCoord;
in vec3 Normal;

vec4 ftransform() {
	return ModelViewProjectionMatrix * Vertex;
}

const float poolHeight = 1.0;

out vec3 position;
void main() {
	position = Vertex.xyz;
	position.x += (abs(position.x) / position.x) * 0.05;
	position.z += (abs(position.z) / position.z) * 0.05;
	position.y = ((1.0 - position.y) * (7.0 / 12.0) - 1.0) * poolHeight;
	position.y += (abs(position.y) / position.y) * 0.05;
	gl_Position = ModelViewProjectionMatrix * vec4(position, 1.0);
}