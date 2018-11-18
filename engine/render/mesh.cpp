#include <phi/render.hpp>
#include <GL/glew.h>
#include "mesh_definitions.h"

using namespace phi;
using namespace phi::render;

struct Quad : public Mesh {
    GLuint VBO = 0, VAO = 0;

    Quad() {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        const GLfloat vertex_data[] = {
                1, 1, -1, 1, 1, -1, -1, -1
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) nullptr);
        glEnableVertexAttribArray(0);
    }

    ~Quad() override {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void drawCall() override {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};

template<const GLfloat vertices[], size_t Bv, const GLushort indices[], size_t Bi>
struct StaticMesh : public Mesh {
    GLuint VBO = 0, IBO = 0, VAO = 0;

    StaticMesh() {
        GLuint buffers[2];
        glGenBuffers(2, buffers);
        VBO = buffers[0];
        IBO = buffers[1];
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Bv, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Bi, indices, GL_STATIC_DRAW);

        const size_t SFLOAT = sizeof(GLfloat);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (3 * SFLOAT));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * SFLOAT, (void *) (6 * SFLOAT));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    ~StaticMesh() override {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void drawCall() override {
        glBindVertexArray(VAO);
        const GLsizei count = Bi / sizeof(GLushort);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const void *) nullptr);
    }
};

std::shared_ptr<Mesh> phi::render::factory::cube() {
    static std::weak_ptr<Mesh> cached{};
    if (cached.expired()) {
        auto mesh_ptr = std::make_shared<StaticMesh<cube_vertices, sizeof(cube_vertices), cube_indices, sizeof(cube_indices)>>();
        cached = mesh_ptr;
        return mesh_ptr;
    } else {
        return cached.lock();
    }
}

std::shared_ptr<Mesh> phi::render::factory::icoSphere() {
    static std::weak_ptr<Mesh> cached{};
    if (cached.expired()) {
        auto mesh_ptr = std::make_shared<StaticMesh<ico_vertices, sizeof(ico_vertices), ico_indices, sizeof(ico_indices)>>();
        cached = mesh_ptr;
        return mesh_ptr;
    } else {
        return cached.lock();
    }
}
