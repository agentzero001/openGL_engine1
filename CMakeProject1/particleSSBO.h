#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <cmath>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Particle {
    // glm::vec3 pos;
    alignas(16) glm::vec3 velocity;
    // glm::vec4 color;
};

class ParticleSSBO {
    public:
        ParticleSSBO(size_t count)
            : m_count(count){ glGenBuffers(1, &m_ssbo); }

        ~ParticleSSBO() { glDeleteBuffers(1, &m_ssbo);}

        template<typename InitFunc>
        void initialize(InitFunc initFunc) {
            std::vector<Particle> particles(m_count);

            for (size_t i = 0; i < m_count; ++i) {
                initFunc(particles[i], i);
            }

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
            glBufferData(
                GL_SHADER_STORAGE_BUFFER,
                particles.size() * sizeof(Particle),
                particles.data(),
                GL_DYNAMIC_DRAW
            );
        }

        void bind(GLuint binding) const {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_ssbo);
        }

        size_t count() const { return m_count; }

    private:
        GLuint m_ssbo = 0;
        size_t m_count = 0;
};