#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer {
    public:
        void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) const;
        void Clear() const;
};