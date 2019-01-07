#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Water.h"
#include "vec3.h"
#include "Water.h"
#include "Cubemap.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    Mesh *waterMesh;
	Mesh *cubeBoxMesh;

    Texture *tileTexture;
    Texture *causticTex;
    
    Vector lightDir;
    float lightDirArray[3];
    float sphereCenterArray[3];
	int clear;

    Shader *waterShader;
    Shader *causticsShader;
	Shader *cubeBoxShader;

    void updateCaustics(Water *water);
    void renderWater(Water *water, Cubemap *sky);
	void renderCubeBox(Water *water);

    void setLightDir(const Vector& v);
	void addLightDir(const Vector& v);

	void toggleClear();
};

