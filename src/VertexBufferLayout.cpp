#include "includes/VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout() {

};

VertexBufferLayout::~VertexBufferLayout() {

};

template <typename T>
void VertexBufferLayout::Push(unsigned int count, unsigned int offset) {
    if (count && offset) return;
};

template <>
void VertexBufferLayout::Push<float>(unsigned int count, unsigned int offset) {
    m_Elements.push_back({GL_FLOAT, count, offset, GL_FALSE});
};

template <>
void VertexBufferLayout::Push<unsigned int>(unsigned int count, unsigned int offset) {
    m_Elements.push_back({GL_UNSIGNED_INT, count, offset, GL_FALSE});
};

template <>
void VertexBufferLayout::Push<unsigned char>(unsigned int count, unsigned int offset) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, offset, GL_TRUE});
};

const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const {
    return m_Elements;
};
