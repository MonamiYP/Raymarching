#pragma once

#include <glad/glad.h>
#include "VertexBuffer.hpp"

class VertexBufferLayout;

class VertexArray {
    private:
        unsigned int m_rendererID;
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

        void Bind() const;
        void Unbind() const;
};