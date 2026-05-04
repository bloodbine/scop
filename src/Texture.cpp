#include "includes/Texture.hpp"

#include "includes/stb_image.h"

void Texture::SetWhiteTexture() {
    uint8_t whitePixel[4] = {255, 255, 255, 255};
    m_Width  = 1;
    m_Height = 1;
    m_BPP    = 4;

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetTexture(std::string& path) {
    m_FilePath = "textures/" + path;

    if (path == "None") {
        SetWhiteTexture();
        return ;
    }

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    if (!m_LocalBuffer) {
        std::cerr << "Failed to load texture: " << m_FilePath << std::endl;
        SetWhiteTexture();
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(m_LocalBuffer);
    m_LocalBuffer = nullptr;
};

Texture::Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SetTexture(m_FilePath);
};

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
};

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
};

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
};

int Texture::GetWidth() const {
    return (m_Width);
};

int Texture::GetHeight() const {
    return (m_Height);
};

int Texture::GetBytesPerPixel() const {
    return (m_BPP);
}

std::string Texture::GetTexturePath() const
{
    return (m_FilePath);
};