#include "Water.h"
#include "GLHeader.h"

Water::Water() {
    plane = Mesh::plane();
	GLint filter = GL_LINEAR;

    Option options;
    options["type"] = toString(GL_FLOAT);
	options["filter"] = toString(filter);
	textureA = new Texture(256, 256, options);
    textureB = new Texture(256, 256, options);
    
    dropShader   = new Shader("./shaders/WaterVertex.vert", "./shaders/WaterDropShader.frag");
    updateShader = new Shader("./shaders/WaterVertex.vert", "./shaders/WaterUpdateShader.frag");
    normalShader = new Shader("./shaders/WaterVertex.vert", "./shaders/WaterNormalShader.frag");
    sphereShader = new Shader("./shaders/WaterVertex.vert", "./shaders/WaterSphereShader.frag");
}

Water::~Water() {
    if (plane) delete plane;
    if (textureA) delete textureA;
    if (textureB) delete textureB;

    if (dropShader) delete dropShader;
    if (updateShader) delete updateShader;
    if (sphereShader) delete sphereShader;
    if (normalShader) delete normalShader;
}

void Water::addDrop(float x, float y, float radius, float strength) {
    float xy[2]; xy[0] = x; xy[1] = y;
	GLint v[4];
	glGetIntegerv(GL_VIEWPORT, v);

    textureB->preDraw(v);
	textureA->bind();
	dropShader->uniforms("center", xy);
	dropShader->uniforms("radius", radius);
	dropShader->uniforms("strength", strength);
	dropShader->uniforms("texture", 0);
	dropShader->draw(plane);

	textureB->postDraw(v);
    textureB->swapWith(textureA);
}

void Water::stepSimulation() {
    float delta[2]; delta[0] = 1.f / float(textureA->width); delta[1] = 1.f / float(textureA->height);
	GLint v[4];
	glGetIntegerv(GL_VIEWPORT, v);

    textureB->preDraw(v);
	textureA->bind();
	updateShader->uniforms("delta", delta);
	updateShader->uniforms("texture", 0);
	updateShader->draw(plane);

	textureB->postDraw(v);
    textureB->swapWith(textureA);
}

void Water::updateNormals() {
    float delta[2]; delta[0] = 1.0f / float(textureA->width); delta[1] = 1.0f / float(textureA->height);
	GLint v[4];
	glGetIntegerv(GL_VIEWPORT, v);

    textureB->preDraw(v);
	textureA->bind();
	normalShader->uniforms("delta", delta);
	normalShader->uniforms("texture", 0);
	normalShader->draw(plane);

	textureB->postDraw(v);
    textureB->swapWith(textureA);
}

