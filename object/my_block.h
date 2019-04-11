
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

#ifndef __MY_BLOCK_H__
#define __MY_BLOCK_H__

#include "block_base.h"

#include <vector>

#include "def.h"
#include "shader.h"

class my_block_t : public block_base_t {
public:
    my_block_t() : block_base_t() {}

    ~my_block_t() {
        for(unsigned i = 0; i < bound_blocks.size(); i++) {
            delete bound_blocks[i];
        }
    }

    my_block_t(float dx, float dy, float dz, float s=1.0f) :
        block_base_t(dx, dy, dz, s) {
    }

private:
    std::vector<block_t*> bound_blocks;
};


#endif
