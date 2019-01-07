#version 330
precision highp float;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewProjectionMatrix;

const float IOR_AIR = 1.0;
const float IOR_WATER = 1.333;
const vec3 abovewaterColor = vec3(0.25, 1.0, 1.25);
const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);
const float poolHeight = 1.0;
uniform sampler2D tiles;

vec3 getWallColor(vec3 point) {
    float scale = 0.5;
    vec3 wallColor;
    vec3 normal;
    if (abs(point.x) > 0.999) {
        wallColor = texture2D(tiles, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
        normal = vec3(-point.x, 0.0, 0.0);
    } else if (abs(point.z) > 0.999) {
        wallColor = texture2D(tiles, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
        normal = vec3(0.0, 0.0, -point.z);
    } else {
        wallColor = texture2D(tiles, point.xz * 0.5 + 0.5).rgb;
        normal = vec3(0.0, 1.0, 0.0);
    }
    return wallColor;
}

in vec3 position;
out vec4 outputF;

void main() {
    outputF = vec4(getWallColor(position), 1.0);
}