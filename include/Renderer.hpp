#pragma once

#include <glad/glad.h>
#include <iostream>
#include <cassert>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

// #define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
    private:
    public:
    void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader) const;
    void Clear() const;
};