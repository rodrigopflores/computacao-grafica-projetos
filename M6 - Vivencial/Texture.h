#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
private:

    GLuint rendererId;
    std::string filepath;
    unsigned char* localBuffer;
    GLint width, height, bpp;

public:

    Texture(const std::string& filepath);
    ~Texture();
    void bind(GLuint slot = 0) const;
    void unbind() const;
    GLint getWidth() const;
    GLint getHeight() const;

};
