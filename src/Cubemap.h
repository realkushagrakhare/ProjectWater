#pragma once

#include "GLHeader.h"
#include "StdHeader.h"

class Cubemap {
public:
    Cubemap(int width[6], int height[6], GLvoid *xn, GLvoid *xp, GLvoid *yn, GLvoid *yp, GLvoid *zn, GLvoid *zp);

    void bind(int unit = 0);
    void unbind(int unit = 0);

    GLuint id;

    static Cubemap* fromImages(const std::string &xneg, const std::string &xpos, const std::string &yneg, const std::string &ypos, const std::string &zneg, const std::string &zpos);
};
