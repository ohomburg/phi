#include <phi/render.hpp>
#include <GL/glew.h>
#include "mesh_definitions.h"

using namespace std;
using namespace phi;
using namespace phi::render;

struct Quad::Impl {
    GLuint VBO, VAO;
};

Quad::Quad() : impl(std::make_unique<Impl>()) {
    glGenBuffers(1, &impl->VBO);
    glGenVertexArrays(1, &impl->VAO);
    glBindVertexArray(impl->VAO);

    const GLfloat vertex_data[] = {
            1, 1, -1, 1, 1, -1, -1, -1
    };

    glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) nullptr);
    glEnableVertexAttribArray(0);
}

void Quad::drawCall() {
    glBindVertexArray(impl->VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Quad::~Quad() = default;

struct IcoSphere::Impl {
    GLuint VBO, IBO, VAO;
};

IcoSphere::IcoSphere() : impl(std::make_unique<Impl>()) {
    glGenBuffers(1, &impl->VBO);
    glGenBuffers(1, &impl->IBO);
    glGenVertexArrays(1, &impl->VAO);
    glBindVertexArray(impl->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ico_vertices), ico_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ico_vertices), ico_indices, GL_STATIC_DRAW);

    const size_t SFLOAT = sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (3 * SFLOAT));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (6 * SFLOAT));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void render::IcoSphere::drawCall() {
    glBindVertexArray(impl->VAO);
    const GLsizei count = sizeof(ico_indices) / sizeof(GLushort);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const void *) nullptr);
}

IcoSphere::~IcoSphere() = default;

struct Cube::Impl {
    GLuint VBO, IBO, VAO;
};

Cube::Cube() : impl(std::make_unique<Impl>()) {
    GLuint buffers[2];
    glGenBuffers(2, buffers);
    impl->VBO = buffers[0];
    impl->IBO = buffers[1];
    glGenVertexArrays(1, &impl->VAO);
    glBindVertexArray(impl->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, impl->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    const size_t SFLOAT = sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (3 * SFLOAT));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (6 * SFLOAT));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void render::Cube::drawCall() {
    glBindVertexArray(impl->VAO);
    const GLsizei count = 36;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const void *) nullptr);
}

Cube::~Cube() = default;
