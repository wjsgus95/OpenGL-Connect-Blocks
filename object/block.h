#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <set>
#include <vector>
#include <cstring>

#include "line.h"

float initial_coords[9][72];

class block_t {
public:
    // Set of blocks you connected.
    static std::set<int> my_blocks;
    // All the blocks on the grid.
    static std::vector<block_t*> all_blocks;
    // Initial set-up.
    static std::vector<block_t*> initial_all_blocks;

    // Game finished/won flags.
    static bool game_done;
    static bool game_won;

    // Number of moves player made.
    static unsigned num_moves;

    line_t lines;
    
    // vertex position array
    GLfloat cubeVertices[72]  = { // initialized as size = 1 for each dimension, 72 elements
        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, // v0,v1,v2,v3 (front)

        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, // v0,v3,v4,v25 (right)

        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, // v0,v25,v6,v1 (top)

        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, // v1,v6,v7,v2 (left)

        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE, BLOCK_HALF_EDGE, // v7,v4,v3,v2 (bottom)

        BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        -BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE,
        BLOCK_HALF_EDGE, BLOCK_HALF_EDGE,-BLOCK_HALF_EDGE  // v4,v7,v6,v25 (back)
    };
    
    // normal array
    GLfloat cubeNormals[72] = {  // 72 elements
        0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // v0,v1,v2,v3 (front)
        1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // v0,v3,v4,v5 (right)
        0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // v0,v5,v6,v1 (top)
        -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  // v1,v6,v7,v2 (left)
        0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // v7,v4,v3,v2 (bottom)
        0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1   // v4,v7,v6,v5 (back)
    };
    
    // colour array
    GLfloat cubeColors[96] = { // initialized as RGBA sollid color for each face, 96 elements
        //0, 0, 0, 1,   0, 0, 0, 1,   0, 0, 0, 1,   0, 0, 0, 1, // v0,v1,v2,v3 (front)
        //1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1,   1, 1, 0, 1, // v0,v3,v4,v5 (right)
        //0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1,   0, 1, 0, 1, // v0,v5,v6,v1 (top)
        //0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1,   0, 1, 1, 1, // v1,v6,v7,v2 (left)
        //0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1,   0, 0, 1, 1, // v7,v4,v3,v2 (bottom)
        //1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1,   1, 0, 1, 1  // v4,v7,v6,v5 (back)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1, // v1,v1,v2,v3 (front)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1, // v1,v3,v4,v5 (right)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1, // v1,v5,v6,v1 (top)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1, // v1,v6,v7,v2 (left)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1, // v7,v4,v3,v2 (bottom)
        1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1  // v4,v7,v6,v5 (back)
    };
    
    // texture coord array
    GLfloat cubeTexCoords[48] = { // 48 elements
        1, 0,   0, 0,   0, 1,   1, 1,               // v0,v1,v2,v3 (front)
        0, 0,   0, 1,   1, 1,   1, 0,               // v0,v3,v4,v5 (right)
        1, 1,   1, 0,   0, 0,   0, 1,               // v0,v5,v6,v1 (top)
        1, 0,   0, 0,   0, 1,   1, 1,               // v1,v6,v7,v2 (left)
        0, 1,   1, 1,   1, 0,   0, 0,               // v7,v4,v3,v2 (bottom)
        0, 1,   1, 1,   1, 0,   0, 0                // v4,v7,v6,v5 (back)
    };
    
    // index array for glDrawElements()
    // A cube requires 36 indices = 6 sides * 2 tris * 3 verts
    GLuint cubeIndices[36] = { // 36 elements
        0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
        4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
        8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
        12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
        16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
        20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
    };


    unsigned id;

    // Block's grid center coords.
    float grid_x;
    float grid_y;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    
    u_int64_t vSize = sizeof(cubeVertices);
    u_int64_t nSize = sizeof(cubeNormals);
    u_int64_t cSize = sizeof(cubeColors);
    u_int64_t tSize = sizeof(cubeTexCoords);
    
    block_t() {
        initBuffers();
    };

    virtual ~block_t() {}
    
    block_t(float dx, float dy, float dz, float s=1.0f) {
        scale(s);
        translate(dx, dy, dz);
        initBuffers();

        grid_x = dx;
        grid_y = dy;

        lines.addLine(dx+BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx+BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT);
        lines.addLine(dx-BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx-BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT);
        lines.addLine(dx+BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx+BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT);
        lines.addLine(dx-BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx-BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT);

        lines.addLine(dx+BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx-BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE);
        lines.addLine(dx-BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx-BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE);
        lines.addLine(dx-BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx+BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE);
        lines.addLine(dx+BLOCK_HALF_EDGE, dy-BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE,
                       dx+BLOCK_HALF_EDGE, dy+BLOCK_HALF_EDGE, TABLE_HEIGHT+BLOCK_EDGE);
    };
    
    void initBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // copy vertex attrib data to VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vSize+nSize+cSize+tSize, 0, GL_STATIC_DRAW); // reserve space
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, cubeVertices);                  // copy verts at offset 0
        glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, cubeNormals);               // copy norms after verts
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize, cSize, cubeColors);          // copy cols after norms
        glBufferSubData(GL_ARRAY_BUFFER, vSize+nSize+cSize, tSize, cubeTexCoords); // copy texs after cols
        
        // copy index data to EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
        
        // attribute position initialization
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)vSize); // normal attrib
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(vSize+nSize)); //color attrib
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+nSize+cSize)); // tex coord
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };
    
    void draw(Shader *block_shader, Shader* line_shader) {
        for(auto it = all_blocks.begin(); it != all_blocks.end(); it++) {
            (*it)->draw_each_block(block_shader, line_shader);
        }
    };

    void draw_each_block(Shader* block_shader, Shader* line_shader) {
        block_shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        lines.draw(line_shader);
    }

    void translate(float dx, float dy, float dz) {
        bool translatable = true;
        for(auto id: my_blocks) {
            block_t* block = all_blocks[id];
            translatable = block->is_block_translatable(dx, dy) && translatable;
        }
        translatable = this->is_block_translatable(dx, dy) && translatable;
        if(!translatable) { 
            //cerr << "block can't move!" << endl;
            return;
        }

        for(auto id: my_blocks) {
            block_t* block = all_blocks[id];
            block->translate_each_block(dx, dy, dz);
        }

        this->translate_each_block(dx, dy, dz);
        num_moves++;
    }

    bool is_block_translatable(float dx, float dy) {
        // Make sure block is on the grid.
        if(cubeVertices[0] + dx < -GRID_START_X || cubeVertices[0] + dx > GRID_START_X+GRID_SIZE) {
            //cout << "block " << id << " off x grid" << endl;
            return false;
        }
        // Make sure block is on the grid.
        if(cubeVertices[1] + dy < (GRID_START_Y) || cubeVertices[1] + dy > -GRID_START_Y+GRID_SIZE) {
            //cout << "block " << id << " off y grid" << endl;
            return false;
        }
        for(auto it = all_blocks.begin(); it != all_blocks.end(); it++) {
            block_t* block = *it;
            if(my_blocks.find(block->id) != my_blocks.end() || block->id == 0) {
                continue;
            }
            if((float)abs((cubeVertices[0] + dx) - block->cubeVertices[0]) < EPSILON &&
               (float)abs((cubeVertices[1] + dy) - block->cubeVertices[1]) < EPSILON) {
                //cout << "block " << id << " and " << block->id << " collision" << endl;
                return false;
            }
        }
        return true;
    }
    
    void translate_each_block(float dx, float dy, float dz) {
        grid_x += dx; grid_y += dy;

        // Make sure block is on the grid.
        if(cubeVertices[0] + dx < -GRID_START_X || cubeVertices[0] + dx > GRID_START_X+GRID_SIZE) {
            return;
        }
        // Make sure block is on the grid.
        if(cubeVertices[1] + dy < (GRID_START_Y) || cubeVertices[1] + dy > -GRID_START_Y+GRID_SIZE) {
            return;
        }
        for (int i = 0; i < 72; i++) {
            if (i % 3 == 0) cubeVertices[i] += dx;
            else if (i % 3 == 1) cubeVertices[i] += dy;
            else if (i % 3 == 2) cubeVertices[i] += dz;
        }

        lines.translate(dx, dy, dz);

        // Update Buffers.
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, cubeVertices);                  // copy verts at offset 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);     // position attrib
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    void move_right() {
        translate(0.0f, GRID_SIZE, 0.0f);
    }
    void move_left() {
        translate(0.0f, -GRID_SIZE, 0.0f);
    }
    void move_up() {
        translate(-GRID_SIZE, 0.0f, 0.0f);
    }
    void move_down() {
        translate(GRID_SIZE, 0.0f, 0.0f);
    }
    
    void scale(float s) {
        for (int i = 0; i < 72; i++)
            cubeVertices[i] *= s;
    };

    // Initialize the game. Place blocks in random locations.
    static void init_random_placement(unsigned m_num_blocks) {
        unsigned num_generated_blocks = 0;
        srand(time(NULL));
        while(num_generated_blocks < m_num_blocks) {
            float grid_x = grid_centers[(rand()) % grid_centers.size()];
            float grid_y = grid_centers[(rand()) % grid_centers.size()];

            // Check collision on generating blocks.
            bool is_colliding = false;
            for(auto it = all_blocks.begin(); it != all_blocks.end(); it++) {
                block_t* block = *it;
                if(abs(grid_x - block->grid_x) < EPSILON &&
                   abs(grid_y - block->grid_y) < EPSILON) {
                    is_colliding = true;
                    //cout << block->id << endl;
                    //cout << "collision: " << grid_x << " " << grid_y << ", " << block->grid_x << " " << block->grid_y << endl;
                }
            }
            if(is_colliding) { continue; }

            block_t* new_block = new block_t(grid_x, grid_y, TABLE_HEIGHT + BLOCK_HALF_EDGE);
            new_block->id = all_blocks.size();
            all_blocks.push_back(new_block);

            memcpy(&initial_coords[num_generated_blocks][0], new_block->cubeVertices, sizeof(float) * 72);
            num_generated_blocks++;
        }

        initial_all_blocks = std::vector<block_t*>(all_blocks);
    }

    // Get starting block.
    static block_t* get_starting_block() {
        // If no block, return.
        if(all_blocks.size() > 0) {
            return all_blocks[0];
        }
        cerr << "Error: No blocks created but requested the starting block." << endl;
        exit(1);
    }

    bool bind() { 
        for(auto id: my_blocks) {
            block_t* my_block = all_blocks[id];
            for(auto it = all_blocks.begin() + 1; it != all_blocks.end(); it++) {
                block_t* block = *it;
                float x = block->cubeVertices[0];
                float y = block->cubeVertices[1];

                if((x-GRID_SIZE == my_block->cubeVertices[0] && y == my_block->cubeVertices[1]) ||
                        (x+GRID_SIZE == my_block->cubeVertices[0] && y == my_block->cubeVertices[1])) {
                    my_blocks.insert(block->id);
                }
                else if ((y-GRID_SIZE == my_block->cubeVertices[1] && x == my_block->cubeVertices[0]) ||
                        (y+GRID_SIZE == my_block->cubeVertices[1] && x == my_block->cubeVertices[0])) {
                    my_blocks.insert(block->id);
                }
            }
        }

        for(auto it = all_blocks.begin() + 1; it != all_blocks.end(); it++) {
            block_t* block = *it;
            float x = block->cubeVertices[0];
            float y = block->cubeVertices[1];

            if((x-GRID_SIZE == cubeVertices[0] && y == cubeVertices[1]) ||
               (x+GRID_SIZE == cubeVertices[0] && y == cubeVertices[1])) {
                my_blocks.insert(block->id);
            }
            else if ((y-GRID_SIZE == cubeVertices[1] && x == cubeVertices[0]) ||
               (y+GRID_SIZE == cubeVertices[1] && x == cubeVertices[0])) {
                my_blocks.insert(block->id);
            }
        }

        //TODO: check if I made the goal shape when all the blocks are connected.
        if(my_blocks.size() == all_blocks.size() - 1) {
            game_done = true;
            // Find corner block coords.
            float min_grid_x = 1e9f, min_grid_y = 1e9f;
            float max_grid_x = -1e9f, max_grid_y = -1e9f;
            for(auto it = all_blocks.begin(); it != all_blocks.end(); it++) {
                block_t* block = *it;
                if(block->grid_x < min_grid_x) {
                    //corner_block_id = block->id;
                    min_grid_x = block->grid_x;
                }
                if(block->grid_y < min_grid_y) {
                    //corner_block_id = block->id;
                    min_grid_y = block->grid_y;
                }
                if(block->grid_x > max_grid_x) {
                    max_grid_x = block->grid_x;
                }
                if(block->grid_y > max_grid_y) {
                    max_grid_y = block->grid_y;
                }
            }
            if(grid_x > max_grid_x) {
                max_grid_x = grid_x;
            }
            if(grid_y > max_grid_y) {
                max_grid_y = grid_y;
            }
            if(grid_x < min_grid_x) {
                min_grid_x = grid_x;
            }
            if(grid_y < min_grid_y) {
                min_grid_y = grid_y;
            }

            // If it's a square.
            if(abs(max_grid_x - min_grid_x - (2*GRID_SIZE)) < EPSILON &&
               abs(max_grid_y - min_grid_y - (2*GRID_SIZE)) < EPSILON) {
                game_won = true;
                cout << "game won!" << endl;
            } else {
                game_won = false;
                cout << "you lost :(" << endl;
            }
            cout << num_moves << " moves in total" << endl;
            return true;
        }
        else {
            return false;
        }
    }

    static block_t* restart_game() {
        all_blocks = initial_all_blocks;
        my_blocks = std::set<int>();
        return all_blocks[0];
    }


private:
};

// Set of blocks you connected to.
std::set<int> block_t::my_blocks = std::set<int>();
// All the blocks on the grid.
std::vector<block_t*> block_t::all_blocks = std::vector<block_t*>();
// Initial block set-up.
std::vector<block_t*> block_t::initial_all_blocks = std::vector<block_t*>();

bool block_t::game_done = false;
bool block_t::game_won = false;

unsigned block_t::num_moves = 0;

#endif
