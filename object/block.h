
// A cube has 6 sides and each side has 4 vertices, therefore, the total number
// of vertices is 24 (6 sides * 4 verts), and 72 floats in the vertex array
// since each vertex has 3 components (x,y,z) (= 24 * 3)
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | v7----|-v4
//  |/      |/
//  v2------v3
//
// Vertex shader: the location (0: position attrib (vec3), 1: normal attrib (vec3),
//                              2: color attrib (vec4), and 3: texture coordinate attrib (vec2))
// Fragment shader: should catch the vertex color from the vertex shader

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "shader.h"
#include "block_base.h"

class block_t : public block_base_t {
public:
    block_t() : block_base_t() {}
    block_t(float dx, float dy, float dz, float s=1.0f) :
        block_base_t(dx, dy, dz, s) {
    }

    void translate(float dx, float dy, float dz) {
        block_base_t::translate(dx, dy, dz);
    }

};


#endif
