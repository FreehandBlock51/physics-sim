#include "viewer/model.h"

#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include "common/defines.h"

/**
 * @brief Gets the size of the C type represented by the given Model Vertex Type
 * @param type The type to get the size of
 * @return The size of the type
 */
PRIVATE_FUNC size_t mvtype_sizeof(mvtype_t type) {
    switch (type) {
        case MVT_FLOAT:
            return sizeof(float);
        case MVT_DOUBLE:
            return sizeof(double);
        case MVT_BYTE:
            return sizeof(int8_t);
        case MVT_UNSIGNED_BYTE:
            return sizeof(uint8_t);
        case MVT_SHORT:
            return sizeof(int16_t);
        case MVT_UNSIGNED_SHORT:
            return sizeof(uint16_t);
        case MVT_INT:
            return sizeof(int32_t);
        case MVT_UNSIGNED_INT:
            return sizeof(uint32_t);
        default:
            abort();
    }
}

/**
 * @brief Calculates the total stride required to hold all of the provided blueprints
 * @param blueprints The blueprints that would be held
 * @param blueprint_count The amount of blueprints that would be held
 * @return The calculated total stride
 */
static size_t mvblueprint_get_total_stride(const mvblueprint_t *blueprints, size_t blueprint_count) {
    size_t stride = 0;

    for (size_t i = 0; i < blueprint_count; i++) {
        mvblueprint_t current_blueprint = blueprints[i];
        stride += current_blueprint.input_size * mvtype_sizeof(current_blueprint.input_type);
    }

    return stride;
}

void mvblueprint_apply_all(const mvblueprint_t *blueprints, size_t blueprint_count, model_t *restrict model) {
    glBindVertexArray(model->VAO);

    size_t stride = mvblueprint_get_total_stride(blueprints, blueprint_count);
    size_t current_offset = 0;

    for (size_t j = 0; j < blueprint_count; j++) {
        mvblueprint_t blueprint = blueprints[j];
        glVertexAttribPointer(j, blueprint.input_size, blueprint.input_type, GL_FALSE, stride, (void*)current_offset);
        glEnableVertexAttribArray(j);

        current_offset += blueprint.input_size * mvtype_sizeof(blueprint.input_type);
    }
}

model_t model_from_vertices(const GLfloat *vertices, size_t vertex_count, const mvblueprint_t *blueprints, size_t blueprint_count, modelbufkind_t buffer_kind, drawstyle_t model_draw_style) {
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
        vertex_count * mvblueprint_get_total_stride(blueprints, blueprint_count) * (sizeof *vertices), // total size of vertex array, in bytes
        vertices, // the vertex array
        buffer_kind); // Information on how often the buffer will be read from and written to

    // set vertex attributes (how shader expects vertex layout)
    mvblueprint_apply_all(blueprints, blueprint_count, &model);

    return model;
}

model_t model_from_indices(
    const GLfloat *vertices,
    size_t vertex_count,
    const mvblueprint_t *blueprints,
    size_t blueprint_count,
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
        vertex_count * mvblueprint_get_total_stride(blueprints, blueprint_count), // total size of vertex array, in bytes
        vertices, // the vertex array
        vertex_buffer_kind); // Information on how often the buffer will be read from and written to

    // put indices in the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * (sizeof *indices), indices, index_buffer_kind);

    // set vertex attributes (how shader expects vertex layout)
    mvblueprint_apply_all(blueprints, blueprint_count, &model);

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
