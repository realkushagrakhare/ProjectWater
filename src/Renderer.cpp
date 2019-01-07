#include "Renderer.h"
#include "Raytracer.h"

Renderer::Renderer() {
    Option options;
    options["minFilter"] = toString(GL_LINEAR_MIPMAP_LINEAR);
    options["warp"] = toString(GL_REPEAT);
    options["format"] = toString(GL_RGB);

    tileTexture = Texture::fromImage("./images/tiles.jpg", options);
    lightDir = Vector(2.0, 2.0, -1.0).unit();
    lightDir.toArray(lightDirArray);
	clear = 0;

    options.clear();
    causticTex = new Texture(1024,1024, options);

    waterMesh = Mesh::plane(200,200);
	waterShader = new Shader("./shaders/RendererWater.vert", "./shaders/RendererWater.frag");
    cubeBoxMesh = Mesh::cube();
    std::vector<unsigned int> new_tri;
    for (int i = 0; i < cubeBoxMesh->triangles.size(); i++) {
        if (i < 12 || i >= 18) new_tri.push_back(cubeBoxMesh->triangles[i]);
    }
    cubeBoxMesh->triangles.swap(new_tri);
    cubeBoxMesh->compile();
    cubeBoxShader = new Shader("./shaders/RendererCubeBox.vert", "./shaders/RendererCubeBox.frag");

    causticsShader = new Shader("./shaders/RendererCaustics.vert", "./shaders/RendererCaustics.frag");

}

Renderer::~Renderer() {
    if (waterMesh) delete waterMesh;
    if (cubeBoxMesh) delete cubeBoxMesh;

    if (tileTexture) delete tileTexture;
    if (causticTex) delete causticTex;

    if (waterShader) delete waterShader;
    if (cubeBoxShader) delete cubeBoxShader;
    if (causticsShader) delete causticsShader;
}

void Renderer::updateCaustics(Water *water) {
	if (!causticsShader) return;
	float *_lightDir = lightDirArray;

	GLint v[4];
	glGetIntegerv(GL_VIEWPORT, v);
	causticTex->preDraw(v);
	water->textureA->bind(0);
	causticsShader->uniforms("light", _lightDir);
	causticsShader->uniforms("water", 0);
	causticsShader->draw(waterMesh);
	causticTex->postDraw(v);

}

void Renderer::renderCubeBox(Water *water) {
	glDisable(GL_CULL_FACE);
    tileTexture->bind(0);
    cubeBoxShader->uniforms("tiles", 0);
    cubeBoxShader->draw(cubeBoxMesh);
}

void Renderer::renderWater(Water *water, Cubemap *sky) {
    RayTracer tracer;
    float eye_array[3]; tracer.eye.toArray(eye_array);
	water->textureA->bind(0);
    tileTexture->bind(1);
    sky->bind(2);
    causticTex->bind(3);
    waterShader->uniforms("light", lightDirArray);
	waterShader->uniforms("water", 0);
	waterShader->uniforms("tiles", 1);
    waterShader->uniforms("sky", 2);
	waterShader->uniforms("causticTex", 3);
    waterShader->uniforms("eye", eye_array);
	waterShader->uniforms("isClear", clear);
	waterShader->draw(waterMesh);
}

void Renderer::setLightDir(const Vector& v) {
    lightDir = v;
    lightDir.toArray(lightDirArray);
}

void Renderer::addLightDir(const Vector& v) {
	lightDir = lightDir.add(v);
	lightDir = lightDir.unit();
	lightDir.toArray(lightDirArray);
}

void Renderer::toggleClear() {
	clear++;
	if (clear == 4)
		clear = 0;
}