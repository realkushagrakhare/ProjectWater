#version 330

const float PI = 3.1415926535897932384626433832795;

const float waveLength = 10.0;
const float waveAmplitude = 1.0;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_indicators;

out vec4 pass_clipSpaceGrid;
out vec4 pass_clipSpaceReal;
out vec3 pass_normal;
out vec3 pass_toCameraVector;

uniform float height;
uniform vec3 cameraPos;
uniform float waveTime;

uniform mat4 projectionViewMatrix;

float generateOffset(float x, float z){
	float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
	float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float xDistortion = generateOffset(vertex.x, vertex.z);
	float yDistortion = generateOffset(vertex.x, vertex.z);
	float zDistortion = generateOffset(vertex.x, vertex.z);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main(void){
	
	vec3 currentVertex = vec3(in_position.x, height, in_position.y);
	
	pass_clipSpaceGrid = projectionViewMatrix * vec4(currentVertex, 1.0);
	
	currentVertex = applyDistortion(currentVertex);
	
	pass_normal = vec3(0.0, 1.0, 0.0);
	
	pass_clipSpaceReal = projectionViewMatrix * vec4(currentVertex, 1.0);
	gl_Position = pass_clipSpaceReal;
	
	pass_toCameraVector = normalize(cameraPos - currentVertex);
	
	

}