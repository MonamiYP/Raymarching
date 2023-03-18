#include "Texture.hpp"
#include "Renderer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_rendererID));
}

Texture::Texture(const std::string& path) 
    : m_rendererID(0), m_filepath(path), m_textureData(nullptr), 
    m_width(0), m_height(0), m_BPP(0) {
        stbi_set_flip_vertically_on_load(1);

        GLCall(glGenTextures(1, &m_rendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        m_textureData = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 0);

         if (m_textureData) {
            GLenum format;
            if (m_BPP == 1) {format = GL_RED;}
            else if (m_BPP == 3) {format = GL_RGB;}
            else {format = GL_RGBA;}
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_textureData));
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }

        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        stbi_image_free(m_textureData);
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}