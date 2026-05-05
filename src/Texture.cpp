#include "includes/Texture.hpp"

#include "includes/stb_image.h"

void Texture::SetWhiteTexture() {
    uint8_t whitePixel[4] = {255, 255, 255, 255};
    m_Width  = 1;
    m_Height = 1;
    m_BPP    = 4;

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetDefaultTexture(int size)
{
    m_Width  = size;
    m_Height = size;
    m_BPP    = 4;

    std::vector<uint8_t> pixels(size * size * 4);
    float maxSum = (float)(size - 1 + size - 1);   // max possible (x+y)

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float t = (float)(x + y) / maxSum;      // 0.0 at top-left, 1.0 at bottom-right
            uint8_t gray = (uint8_t)(t * 255.0f);

            size_t idx = (y * size + x) * 4;
            pixels[idx + 0] = gray;   // R
            pixels[idx + 1] = gray;   // G
            pixels[idx + 2] = gray;   // B
            pixels[idx + 3] = 255;    // A
        }
    }

    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetTexture(std::string& path) {
    if (path == "Default") {
        SetDefaultTexture(256);
        return ;
    }

    if (path == "None") {
        SetWhiteTexture();
        return ;
    }

    m_FilePath = "textures/" + path;

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