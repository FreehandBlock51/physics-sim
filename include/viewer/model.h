#pragma once

/**
 * Utilities for drawing objects
 */

#include <stdbool.h>
#include <gl_includes.h>

/**
 * @brief Represents the type a model's vertices will be interpreted as
 */
enum ModelVertexType {
    /**
     * @brief This input consists of single-precision floating-point numbers
     */
    MVT_FLOAT = GL_FLOAT,
    /**
     * @brief This input consists of double-precision floating-point numbers
     */
    MVT_DOUBLE = GL_DOUBLE,
    /**
     * @brief This input consists of bytes
     */
    MVT_BYTE = GL_BYTE,
    /**
     * @brief This input consists of unsigned bytes
     */
    MVT_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    /**
     * @brief This input consists of short integers
     */
    MVT_SHORT = GL_SHORT,
    /**
     * @brief This input consists of unsigned short integers
     */
    MVT_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
    /**
     * @brief This input consists of integers
     */
    MVT_INT = GL_INT,
    /**
     * @brief This input consists of unsigned integers
     */
    MVT_UNSIGNED_INT = GL_UNSIGNED_INT,
};
typedef enum ModelVertexType mvtype_t;

/**
 * @brief The layout of an input to a vertex shader.  Used to configure a model
 * @link https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
 */
struct ModelVertexBlueprint {
    /**
     * The type of items this blueprint will contain.  For example, a shader
     * input with a type of `float4` in GLSL will correspond to a blueprint of
     * type `MVT_FLOAT`
     */
    mvtype_t input_type;
    /**
     * The amount of items this blueprint will contain.  For example, a shader
     * input with a type of `float4` in GLSL will correspond to a blueprint of
     * size `4`
     */
    uint8_t input_size;
};
typedef struct ModelVertexBlueprint mvblueprint_t;

/**
 * Creates a blueprint with the given type and size
 */
#define mvblueprint_make(type, size) ((mvblueprint_t){ .input_size = size, .input_type = type })

/**
 * @brief Information on how a model's buffer will be used after
 * it is bound to the model.
 * @link https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
 */
enum ModelBufferKind {
    /**
     * The buffer WILL NOT be modified after binding,
     * and the model will be short-lived
     */
    MODEL_BUFFER_STREAM = GL_STREAM_DRAW,
    /**
     * The buffer WILL NOT be modified after binding,
     * and the model will be long-lived
     */
    MODEL_BUFFER_STATIC = GL_STATIC_DRAW,
    /**
     * The buffer WILL be modified after binding,
     * and the model will be long-lived
     */
    MODEL_BUFFER_DYNAMIC = GL_DYNAMIC_DRAW,
};
typedef enum ModelBufferKind modelbufkind_t;

/**
 * @brief Various ways OpenGL can interpret a model's
 * vertices when drawing
 */
enum ModelDrawStyle {
    /**
     * Draw a point at the location specified by each
     * individual vertex
     */
    MODEL_DRAW_POINTS = GL_POINTS,
    /**
     * Draw a line connecting the locations specified
     * by each pair of vertices
     */
    MODEL_DRAW_LINES = GL_LINES,
    /**
     * Draw triangles connecting the locations specified
     * by each group of three vertices
     */
    MODEL_DRAW_TRIANGLES = GL_TRIANGLES,
    /**
     * Draw a quadrilateral connecting the locations
     * specified by each group of four vertices
     */
    MODEL_DRAW_QUADS = GL_QUADS,
};
typedef enum ModelDrawStyle drawstyle_t;

/**
 * @brief Represents a model that can be rendered in OpenGL
 */
struct Model {
    /**
     * The Vertex Array Object (VAO) OpenGL uses to draw the
     * model
     */
    GLuint VAO;
    /**
     * The Vertex Buffer Object (VBO) that contains the
     * vertices OpenGL will draw
     */
    GLuint VBO;
    /**
     * The Element Buffer Object (EBO) that contains the indices
     * of vertices OpenGL will draw.  If no index buffer was
     * provided, this value is unused
     */
    GLuint EBO;
    /**
     * Determines how OpenGL will draw the vertices.  For example,
     * a value of MODEL_DRAW_TRIANGLES will cause OpenGL to draw triangles
     * using each set of three provided vertices
     */
    drawstyle_t model_draw_style;
    /**
     * How many vertices to draw.  If this model uses an index buffer,
     * this value refers to the size of that buffer; otherwise, it
     * refers to the size of the vertex buffer
     */
    GLsizei model_size;
    /**
     * If true, the model won't generate an index buffer and will only
     * pass its vertex buffer when drawing
     */
    bool vertex_only;
};
typedef struct Model model_t;

/**
 * @brief Creates a model from a collection of vertices
 * @param vertices The array of vertices to load
 * @param vertex_count How many vertices should be loaded
 * @param blueprints The blueprints to apply to the model
 * @param blueprint_count How many blueprints will be applied
 * @param buffer_kind How will the vertex buffer be used after this model is created?
 * @param model_draw_style How should OpenGL render this model? (e.g. GL_TRIANGLES)
 * @return A model representing the given vertices
 */
model_t model_from_vertices(const GLfloat *vertices, size_t vertex_count, const mvblueprint_t *blueprints, size_t blueprint_count, modelbufkind_t buffer_kind, drawstyle_t model_draw_style);

/**
 * @brief Creates a model from a collection of both vertices and indices
 * @param vertices The vertices to load
 * @param vertex_count How many vertices should be loaded
 * @param blueprints The blueprints to apply to the model
 * @param blueprint_count How many blueprints will be applied
 * @param vertex_buffer_kind How will the vertex buffer be used after this model is created?
 * @param indices The indices to load
 * @param index_count How many indices should be loaded
 * @param index_buffer_kind How will the index buffer be used after this model is created?
 * @param model_draw_style How should OpenGL render this model? (e.g. GL_TRIANGLES)
 * @return A model representing the given vertices and indices
 */
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
);

/**
 * @brief Applies all blueprint to a given model
 * @param blueprints An array containing the blueprints to apply
 * @param blueprint_count The amount of blueprints to apply
 * @param model The model to apply the blueprint to
 */
void mvblueprint_apply_all(const mvblueprint_t *blueprints, size_t blueprint_count, model_t *model);

/**
 * @brief Draws the given model
 * @param model The model to draw
 */
void model_draw(model_t model);
