#include <GL/glew.h>
#include "stb_image/stb_image.h"

#include "Texture.h"
#include "error_handling.h"


Texture::Texture(const std::string& path)
: m_RendererId(0), m_FilePath(path), m_LocalBuffer(nullptr), 
  m_Width(0), m_Height(0), m_BPP(0) {
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (m_LocalBuffer)
    {
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        stbi_image_free(m_LocalBuffer);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(GL_TEXTURE_2D, &m_RendererId));
}

void Texture::Bind(const unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}