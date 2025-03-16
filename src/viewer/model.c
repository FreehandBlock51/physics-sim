#include "viewer/model.h"

model_t model_from_vertices(const GLfloat *vertices, size_t vertex_stride, size_t vertex_count, modelbufkind_t buffer_kind, drawstyle_t model_draw_style) {
    model_t model = {
        .VAO = 0, // all GL handles set to zero, initialized later in this function
        .VBO = 0,
        .EBO = 0,
        .model_draw_style = model_draw_style,
        .model_size = vertex_count,
        .vertex_only = true
    };

    // Create a Vertex Array Object (VAO) to store the environment we
    // need to draw (vertices, vertex attributes, etc.)
    glGenVertexArrays(1, &model.VAO);

    glBindVertexArray(model.VAO);

    // Create a Vertex Buffer Object (VBO) to store our vertices
    glGenBuffers(1, &model.VBO);

    // Don't create an Element Buffer Object because we aren't using an index buffer

    // put vertices in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertex_count * vertex_stride * (sizeof *vertices), // total size of vertex array, in bytes
        vertices, // the vertex array
        buffer_kind); // Information on how often the buffer will be read from and written to

    // set vertex attributes (how shader expects vertex layout)
    glVertexAttribPointer(0, vertex_stride, GL_FLOAT, GL_FALSE, vertex_stride * (sizeof *vertices), NULL);
    glEnableVertexAttribArray(0);

    return model;
}

model_t model_from_indices(
    const GLfloat *vertices,
    size_t vertex_stride,
    size_t vertex_count,
    modelbufkind_t vertex_buffer_kind,
    const GLuint *indices,
    size_t index_count,
    modelbufkind_t index_buffer_kind,
    drawstyle_t model_draw_style
) {
    model_t model = {
        .VAO = 0, // all GL handles set to zero, initialized later in this function
        .VBO = 0,
        .EBO = 0,
        .model_draw_style = model_draw_style,
        .model_size = index_count,
        .vertex_only = false
    };

    // Create a Vertex Array Object (VAO) to store the environment we
    // need to draw (vertices, vertex attributes, etc.)
    glGenVertexArrays(1, &model.VAO);

    glBindVertexArray(model.VAO);

    // Create a Vertex Buffer Object (VBO) to store our vertices
    glGenBuffers(1, &model.VBO);

    // Create an Element Buffer Object (EBO) to store our indices
    glGenBuffers(1, &model.EBO);

    // put vertices in the VBO
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertex_count * vertex_stride * (sizeof *vertices), // total size of vertex array, in bytes
        vertices, // the vertex array
        vertex_buffer_kind); // Information on how often the buffer will be read from and written to

    // put indices in the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * (sizeof *indices), indices, index_buffer_kind);

    // set vertex attributes (how shader expects vertex layout)
    glVertexAttribPointer(0, vertex_stride, GL_FLOAT, GL_FALSE, vertex_stride * (sizeof *vertices), NULL);
    glEnableVertexAttribArray(0);

    return model;
}

void model_draw(model_t model) {
    glBindVertexArray(model.VAO);

    if (model.vertex_only) {
        // only use the vertex buffer
        glDrawArrays(model.model_draw_style, 0, model.model_size);
    }
    else {
        // use the element buffer and vertex buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
        glDrawElements(model.model_draw_style, model.model_size, GL_UNSIGNED_INT, 0);
    }

}
