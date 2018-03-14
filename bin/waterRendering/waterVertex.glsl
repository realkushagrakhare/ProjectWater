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

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2){
	vec3 tangent = vertex1 - vertex0;
	vec3 bitangent = vertex2 - vertex0;
	return normalize(cross(tangent, bitangent));
}

float generateOffset(float x, float z){
	float radiansX = (x / waveLength + waveTime) * 2.0 * PI;
	float radiansZ = (z / waveLength + waveTime) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
	float yDistortion = generateOffset(vertex.x, vertex.z);
	float xDistortion = generateOffset(vertex.x, vertex.z);
	float zDistortion = generateOffset(vertex.x, vertex.z);
	return vertex + vec3(xDistortion, yDistortion, zDistortion);
}

void main(void){
	
	vec3 currentVertex = vec3(in_position.x, height, in_position.y);
	vec3 vertex1 = currentVertex + vec3(in_indicators.x, 0.0, in_indicators.y);
	vec3 vertex2 = currentVertex + vec3(in_indicators.z, 0.0, in_indicators.w);
	
	pass_clipSpaceGrid = projectionViewMatrix * vec4(currentVertex, 1.0);
	
	currentVertex = applyDistortion(currentVertex);
	vertex1 = applyDistortion(vertex1);
	vertex2 = applyDistortion(vertex2);
	
	pass_normal = calcNormal(currentVertex, vertex1, vertex2);
	
	pass_clipSpaceReal = projectionViewMatrix * vec4(currentVertex, 1.0);
	gl_Position = pass_clipSpaceReal;
	
	pass_toCameraVector = normalize(cameraPos - currentVertex);
	
	

}