#version 330

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_indicators;

out vec4 pass_clipSpaceReal;
out vec3 pass_normal;
out vec3 pass_toCameraVector;

uniform float height;
uniform vec3 cameraPos;

uniform mat4 projectionViewMatrix;

void main(void){
	
	vec3 currentVertex = vec3(in_position.x, height, in_position.y);
	
	pass_normal = vec3(0.0, 1.0, 0.0);
	
	pass_clipSpaceReal = projectionViewMatrix * vec4(currentVertex, 1.0);
	gl_Position = pass_clipSpaceReal;
	
	pass_toCameraVector = normalize(cameraPos - currentVertex);
	
	

}