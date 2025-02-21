#include "viewer/aabb.h"

void bbox_make_transform(bbox_t box, mat4 transform) {
    glm_mat4_identity(transform);
    glm_translate(transform, vec3_to_cglm(box.position));
}

/* Vertex array locations */
#define BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK    0
#define BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK   1
#define BBOX_VERTEX_LOCATION_LEFT_TOP_BACK       2
#define BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK      3
#define BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT   4
#define BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT  5
#define BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT      6
#define BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT     7

/**
 * Populates a field of a bounding box's vertex array with the given value
 */
#define BBOX_SET_VERTEX_FIELD(vertex_array, location, offset, value) vertex_array[((location) * 3) + (offset)] = value

void bbox_gen_vertices(bbox_t box, GLfloat *vertices, GLint *indices) {
    // generate vertices for all of the box's corners

    // (left, bottom, back)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_X, box.left);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_Y, box.bottom);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_Z, box.back);

    // (right, bottom, back)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_X, box.right);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_Y, box.bottom);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK, VIEWER_VERTEX_OFFSET_Z, box.back);

    // (left, top, back)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_BACK, VIEWER_VERTEX_OFFSET_X, box.left);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_BACK, VIEWER_VERTEX_OFFSET_Y, box.top);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_BACK, VIEWER_VERTEX_OFFSET_Z, box.back);

    // (right, top, back)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK, VIEWER_VERTEX_OFFSET_X, box.right);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK, VIEWER_VERTEX_OFFSET_Y, box.top);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK, VIEWER_VERTEX_OFFSET_Z, box.back);

    // (left, bottom, front)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_X, box.left);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_Y, box.bottom);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_Z, box.front);

    // (right, bottom, front)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_X, box.right);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_Y, box.bottom);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT, VIEWER_VERTEX_OFFSET_Z, box.front);

    // (left, top, front)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT, VIEWER_VERTEX_OFFSET_X, box.left);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT, VIEWER_VERTEX_OFFSET_Y, box.top);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT, VIEWER_VERTEX_OFFSET_Z, box.front);

    // (right, top, front)
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT, VIEWER_VERTEX_OFFSET_X, box.right);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT, VIEWER_VERTEX_OFFSET_Y, box.top);
    BBOX_SET_VERTEX_FIELD(vertices, BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT, VIEWER_VERTEX_OFFSET_Z, box.front);


    // generate indices -- each face is made up of two triangles

    // front face (top right)
    indices[0] = BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT; // (right, top, front)
    indices[1] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT; // (right, bottom, front)
    indices[2] = BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT; // (left, top, front)
    
    // front face (bottom left)
    indices[3] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT; // (left, bottom, front)
    indices[4] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT; // (right, bottom, front)
    indices[5] = BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT; // (left, top, front)

    // back face (top right)
    indices[6] = BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK; // (right, top, back)
    indices[7] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK; // (right, bottom, back)
    indices[8] = BBOX_VERTEX_LOCATION_LEFT_TOP_BACK; // (left, top, back)
    
    // back face (bottom left)
    indices[9] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK; // (left, bottom, back)
    indices[10] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK; // (right, bottom, back)
    indices[11] = BBOX_VERTEX_LOCATION_LEFT_TOP_BACK; // (left, top, back)

    // left face (front top)
    indices[12] = BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT; // (left, top, front)
    indices[13] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT; // (left, bottom, front)
    indices[14] = BBOX_VERTEX_LOCATION_LEFT_TOP_BACK; // (left, top, back)

    // left face (back bottom)
    indices[15] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK; // (left, bottom, back)
    indices[16] = BBOX_VERTEX_LOCATION_LEFT_TOP_BACK; // (left, top, back)
    indices[17] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT; // (left, bottom, front)

    // right face (front top)
    indices[18] = BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT; // (right, top, front)
    indices[19] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT; // (right, bottom, front)
    indices[20] = BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK; // (right, top, back)

    // right face (back bottom)
    indices[21] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK; // (right, bottom, back)
    indices[22] = BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK; // (right, top, back)
    indices[23] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT; // (right, bottom, front)

    // top face (back left)
    indices[24] = BBOX_VERTEX_LOCATION_LEFT_TOP_BACK; // (left, top, back)
    indices[25] = BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT; // (left, top, front)
    indices[26] = BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK; // (right, top, back)

    // top face (front right)
    indices[27] = BBOX_VERTEX_LOCATION_RIGHT_TOP_FRONT; // (right, top, front)
    indices[28] = BBOX_VERTEX_LOCATION_RIGHT_TOP_BACK; // (right, top, back)
    indices[29] = BBOX_VERTEX_LOCATION_LEFT_TOP_FRONT; // (left, top, front)

    // bottom face (back left)
    indices[30] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_BACK; // (left, bottom, back)
    indices[31] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT; // (left, bottom, front)
    indices[32] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK; // (right, bottom, back)

    // bottom face (front right)
    indices[33] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_FRONT; // (right, bottom, front)
    indices[34] = BBOX_VERTEX_LOCATION_RIGHT_BOTTOM_BACK; // (right, bottom, back)
    indices[35] = BBOX_VERTEX_LOCATION_LEFT_BOTTOM_FRONT; // (left, bottom, front)
}
