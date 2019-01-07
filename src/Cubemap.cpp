#include "Cubemap.h"
#include "SOIL.h"

Cubemap::Cubemap(int width[6], int height[6], GLvoid *xn, GLvoid *xp, GLvoid *yn, GLvoid *yp, GLvoid *zn, GLvoid *zp) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    //gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // !!!
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, xn);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, xp);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width[2], height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, yn);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, yp);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, zn);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width[5], height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, zp);
}

void Cubemap::bind(int unit /* = 0 */) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void Cubemap::unbind(int unit /* = 0 */) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap *Cubemap::fromImages(const std::string &xneg, const std::string &xpos, const std::string &yneg, const std::string &ypos, const std::string &zneg, const std::string &zpos) {
    int w[6], h[6];
    unsigned char* xn = SOIL_load_image(xneg.c_str(), &w[0], &h[0], 0, SOIL_LOAD_RGB);
    unsigned char* xp = SOIL_load_image(xpos.c_str(), &w[1], &h[1], 0, SOIL_LOAD_RGB);
    unsigned char* yn = SOIL_load_image(yneg.c_str(), &w[2], &h[2], 0, SOIL_LOAD_RGB);
    unsigned char* yp = SOIL_load_image(ypos.c_str(), &w[3], &h[3], 0, SOIL_LOAD_RGB);
    unsigned char* zn = SOIL_load_image(zneg.c_str(), &w[4], &h[4], 0, SOIL_LOAD_RGB);
    unsigned char* zp = SOIL_load_image(zpos.c_str(), &w[5], &h[5], 0, SOIL_LOAD_RGB);

    Cubemap *cubemap = new Cubemap(w, h, xn, xp, yn, yp, zn, zp);
    SOIL_free_image_data(xn);
	SOIL_free_image_data(xp);
    SOIL_free_image_data(yn);
    SOIL_free_image_data(yp);
    SOIL_free_image_data(zn);
    SOIL_free_image_data(zp);
    return cubemap;
}
