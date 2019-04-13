
// line_t
//
// Vertex shader: the location (0: position attrib (vec3), 1: color (vec3))
// Fragment shader: should catch the vertex color from the vertex shader

#ifndef __LINE_H__
#define __LINE_H__

#include <cmath>
#include <iostream>
#include "shader.h"

using namespace std;


class line_t {
    
public:
    const int MAX_LINE = 100;
    
    line_t() {
        numLines = numVertices = 0;
        colorIndex = 0;
        createBuffers();
        updateBuffers();
    }
    
    // add a new line
    void addLine(float x1, float y1, float z1, float x2, float y2, float z2) {
        int k = numVertices;
        vertices[k*6] = x1;
        vertices[k*6+1] = y1;
        vertices[k*6+2] = z1;
        vertices[k*6+3] = x2;
        vertices[k*6+4] = y2;
        vertices[k*6+5] = z2;
        colors[k*6] = mainColors[colorIndex * 3];
        colors[k*6+1] = mainColors[colorIndex * 3 + 1];
        colors[k*6+2] = mainColors[colorIndex * 3 + 2];
        colors[k*6+3] = colors[k*6];
        colors[k*6+4] = colors[k*6+1];
        colors[k*6+5] = colors[k*6+2];
        numLines++;
        if (numLines > MAX_LINE) {
            cout << "addLine(line_t) error: maximum lines over" << endl;
            exit(-1);
        }
        numVertices += 2;
        colorIndex = (colorIndex + 1) % 7;
        updateBuffers(); 
    }
    
    // get two end points of k'th line
    void getEndPoints(int k, float &x1, float &y1, float &z1,
                             float &x2, float &y2, float &z2) {
        k = k * 6;
        x1 = vertices[k];
        y1 = vertices[k+1];
        z1 = vertices[k+2];
        x2 = vertices[k+3];
        y2 = vertices[k+4];
        z2 = vertices[k+5]; 
    }
    
    void draw(Shader *shader) {
        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, numVertices*2 - 1);
        glBindVertexArray(0);
    };

    void translate(float dx, float dy, float dz) {
        for(unsigned k = 0; k < numVertices; k++) {
            vertices[k*6] += dx;
            vertices[k*6+3] += dx;
            vertices[k*6+1] += dy;
            vertices[k*6+4] += dy;
            vertices[k*6+2] += dz;
            vertices[k*6+5] += dz;
        }
        updateBuffers();
    }

private:
    
    int numLines;
    int numVertices;
    GLfloat vertices[600];  // 100 * 2 * 3
    GLfloat colors[600];    // 100 * 2 * 3
    
    int colorIndex;
    
    float mainColors[21] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };
    
    unsigned int VAO;
    unsigned int VBO[2];      // VBO[0]: for position, VBO[1]: for color
    
    void createBuffers() {
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(2, VBO);
        
        glBindVertexArray(VAO);
        
        // reserve space for position attributes
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // reserve space for color attributes
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(0);
        
    }
    
    void updateBuffers() {
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(0);
    };
    
};


#endif
