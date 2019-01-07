#include "Texture.h"
#include "SOIL.h"

Texture::Texture(int w, int h, Option &options)
    : width(w), height(h)
{
    glGenTextures(1, &id);

    format = GL_RGBA;
    type = GL_UNSIGNED_BYTE;
    magFilter = GL_LINEAR;
    minFilter = GL_LINEAR;
    GLenum warp = GL_CLAMP_TO_EDGE, warpS = GL_CLAMP_TO_EDGE, warpT = GL_CLAMP_TO_EDGE;
    
    if (options.count("format") > 0) format = parseTo<GLenum>(options["format"]);
    if (options.count("type") > 0) type = parseTo<GLenum>(options["type"]);
    
    if (options.count("warpS") > 0) warpS = parseTo<GLenum>(options["warpS"]);
    if (options.count("warpT") > 0) warpT = parseTo<GLenum>(options["warpT"]);
    if (options.count("warp") > 0) warp = warpS = warpT = parseTo<GLenum>(options["warp"]);

    if (options.count("magFilter") > 0) magFilter = parseTo<GLint>(options["magFilter"]);
    if (options.count("minFilter") > 0) minFilter = parseTo<GLint>(options["minFilter"]);
    if (options.count("filter") > 0) magFilter = minFilter = parseTo<GLint>(options["filter"]);

    //glGenFramebuffers(1, &framebuffer);
    //glGenRenderbuffers(1, &renderbuffer);
    framebuffer = 0;
    renderbuffer = 0;

    glBindTexture(GL_TEXTURE_2D, id);
    // TODO gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpT);
    
    if ( type == GL_FLOAT){
        float *f = new float[width*height*4];
        for (int i = 0; i < width*height*4; i++) {
            f[i] = 0.f;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, format, type, f);
        delete [] f;
    }
    else{
        unsigned char *uc = new unsigned char[width*height*4];
        for (int i = 0; i < width*height*4; i++) {
            uc[i] = 0;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, uc);
        delete [] uc;
    }

}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::canDraw() {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, id, 0);
    bool result = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT;
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    return result;
}

void Texture::preDraw(GLint v[4]) {
    glGetIntegerv(GL_VIEWPORT, v);
    /*if (!framebuffer) */glGenFramebuffersEXT(1, &framebuffer);
    /*if (!renderbuffer) */glGenRenderbuffersEXT(1, &renderbuffer);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
    GLint renderbuffer_width, renderbuffer_height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &renderbuffer_width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &renderbuffer_height);
    if (width != renderbuffer_width || height != renderbuffer_height) {
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
    }
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, id, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer);
    if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
        std::cerr << "Rendering to this texture is not supported (incomplete framebuffer)" << std::endl;
    } 
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Texture::postDraw(GLint v[4]) {
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    glViewport(v[0], v[1], v[2], v[3]);
}

Texture *Texture::fromImage(const std::string &path, Option &options) {
    int w, h;
    unsigned char* image = SOIL_load_image(path.c_str(), &w, &h, 0, SOIL_LOAD_RGB);

    Texture *texture = new Texture(w, h, options);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->format, texture->width, texture->height, 0, texture->format, texture->type, image);
    
    if (texture->minFilter != GL_NEAREST && texture->minFilter != GL_LINEAR) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    SOIL_free_image_data(image);
    return texture;
}

Texture::~Texture() {}

void Texture::swapWith(Texture *that) {
    GLuint tempId = that->id; 
    that->id = this->id; 
    this->id = tempId;

    int tempW = that->width;
    that->width = this->width;
	this->width = tempW;

    int tempH = that->height;
    that->height = this->height;
    this->height = tempH;
}

void Texture::saveToFile(const std::string &path) {
	unsigned char *img = new unsigned char[width*height * 3];
	glBindTexture(GL_TEXTURE_2D, id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_save_image(path.c_str(), SOIL_SAVE_TYPE_BMP, width, height, 3, img);
	delete[] img;
}
