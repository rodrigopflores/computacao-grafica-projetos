#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture() : rendererId(0), filepath(""), localBuffer(nullptr), width(0), height(0), bpp(0) { }

Texture::Texture(const std::string& filepath) : filepath(filepath) {
    load(filepath);
}

Texture::~Texture() {
    glDeleteTextures(1, &rendererId);
}

void Texture::load(const std::string& filepath) {
    //stbi_set_flip_vertically_on_load(1);

    this->filepath = filepath;
    localBuffer = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);
    glGenTextures(1, &rendererId);
    glBindTexture(GL_TEXTURE_2D, rendererId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
    unbind();

    if (localBuffer) {
        stbi_image_free(localBuffer);
    }
}

void Texture::bind(GLuint slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, rendererId);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLint Texture::getWidth() const {
    return width;
}

GLint Texture::getHeight() const {
    return height;
}
