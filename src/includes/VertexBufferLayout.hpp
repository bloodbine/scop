#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int offset;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        return 0;
    };
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferElement> m_Elements;

    public:
        VertexBufferLayout();
        ~VertexBufferLayout();

    template <typename T>
    void Push(unsigned int count, unsigned int offset);
    template <>
    void Push<float>(unsigned int count, unsigned int offset);
    template <>
    void Push<unsigned int>(unsigned int count, unsigned int offset);
    template <>
    void Push<unsigned char>(unsigned int count, unsigned int offset);

    const std::vector<VertexBufferElement>& GetElements() const;
};
