#pragma once

#include "GLHeader.h"
#include "StdHeader.h"

class Texture {
public:
    Texture(int w, int h, Option &options);
    ~Texture();

    void bind(int unit = 0);
    void unbind(int unit = 0);
    bool canDraw();
    void swapWith(Texture *that);
	void saveToFile(const std::string &path);

    void preDraw(GLint v[4]);
    void postDraw(GLint v[4]);

    GLuint id;

    int width, height;

    GLenum format;
    GLenum type;
    GLint magFilter, minFilter;

    static Texture *fromImage(const std::string &path, Option &options);
private:
    GLuint framebuffer, renderbuffer;
};