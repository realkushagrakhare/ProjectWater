#pragma once

#include "StdHeader.h"
#include "GLHeader.h"
#include "vsShaderLib.h"
#include "Mesh.h"

class Shader{
public:
    // now the vertex and fragment source mean the file path
    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    ~Shader();

    //GLuint program;
    VSShaderLib vsshader;

    void draw(Mesh *mesh);
    void uniforms(const std::string &name, void *val);
    void uniforms(const std::string &name, float val);
    void uniforms(const std::string &name, int val);

private:
};