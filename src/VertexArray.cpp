#include "includes/VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);
};

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
};

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const std::vector<VertexBufferElement>& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            0,
            reinterpret_cast<void *>(static_cast<uintptr_t>(element.offset))
        );
    }
};

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
};

void VertexArray::UnBind() const {
    glBindVertexArray(0);
};