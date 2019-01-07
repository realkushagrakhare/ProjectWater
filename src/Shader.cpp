#include "Shader.h"
#include "vsMathLib.h"

extern VSMathLib *vsml;

Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) {
    vsshader.init();
    vsshader.loadShader(VSShaderLib::VERTEX_SHADER, vertexSource);
    vsshader.loadShader(VSShaderLib::FRAGMENT_SHADER, fragmentSource);
    vsshader.setProgramOutput(0,"outputF");

    vsshader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "Vertex");
    vsshader.setVertexAttribName(VSShaderLib::TEXTURE_COORD_ATTRIB, "TexCoord");
    vsshader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "Normal");
    
    vsshader.prepareProgram();
    
}

void Shader::draw(Mesh *mesh) {
    // set the uniforms
    glUseProgram(vsshader.getProgramIndex());
    vsml->setUniformName(VSMathLib::NORMAL, "NormalMatrix");
    vsml->setUniformName(VSMathLib::VIEW_MODEL, "ModelViewMatrix");
    vsml->setUniformName(VSMathLib::PROJECTION, "ProjectionMatrix");
    vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "ModelViewProjectionMatrix");
    vsml->matricesToGL();
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->triangles.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Shader::uniforms(const std::string &name, void *val) {
    glUseProgram(vsshader.getProgramIndex());
	vsshader.setUniform(name, val);
}

void Shader::uniforms(const std::string &name, float val) {
    glUseProgram(vsshader.getProgramIndex());
    vsshader.setUniform(name, val);
}

void Shader::uniforms(const std::string &name, int val) {
    glUseProgram(vsshader.getProgramIndex());
    vsshader.setUniform(name, val);
}

Shader::~Shader() {}
