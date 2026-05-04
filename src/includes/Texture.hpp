#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Texture {
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width;
        int m_Height;
        int m_BPP;

        void SetWhiteTexture();

    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void UnBind() const;

        int GetWidth() const;
        int GetHeight() const;
        int GetBytesPerPixel() const;
        std::string GetTexturePath() const;

        void SetTexture(std::string& path);
};