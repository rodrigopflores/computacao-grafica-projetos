#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string &filepath)
    : filepath(filepath) {

    stbi_set_flip_vertically_on_load(1);
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

Texture::~Texture() {
    glDeleteTextures(1, &rendererId);
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
