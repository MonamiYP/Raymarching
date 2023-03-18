#pragma once

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

struct VertexBufferAttributes {
    unsigned int type;
    unsigned int size;
    unsigned char normalised;
};

class VertexBufferLayout {
    private:
        std::vector<VertexBufferAttributes> m_attributes;
        unsigned int m_stride;
    public:
        VertexBufferLayout() : m_stride(0) {}

        void AddAttribute(unsigned int size) {
            m_attributes.push_back({ GL_FLOAT, size, GL_FALSE });
            m_stride += sizeof(GLfloat) * size;
        }

        void IgnoreAttribute(unsigned int size) {
            m_stride += sizeof(GLfloat) * size;
        }

        inline const std::vector<VertexBufferAttributes> GetAttributes() const& { return m_attributes; }
        inline unsigned int GetStride() const { return m_stride; }
};