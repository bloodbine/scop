#pragma once

class VertexBuffer {
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer(unsigned int size);
        ~VertexBuffer();

        void AddSubData(const void* data, unsigned int size, unsigned int offset);
        void Bind() const;
        void UnBind() const;
};