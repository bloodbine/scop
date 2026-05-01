#include "includes/VertexBuffer.hpp"
#include "includes/VertexBufferLayout.hpp"

VertexBuffer::VertexBuffer(unsigned int size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
};

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
};

void VertexBuffer::AddSubData(const void* data, unsigned int size, unsigned int offset) {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
};

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
};

void VertexBuffer::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};