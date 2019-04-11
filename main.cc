// 20_rotor_t
//      Keyboard: r: reset the arc ball to initial state
//                a: toggle camera rotation and model rotation mode
//      Mouse: left button: begin arc ball dragging

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>

#include <shader.h>
#include <getbmp.h>

#include "def.h"
#include "cube.h"
#include "myblock.h"
#include "table.h"
#include "rotor.h"



using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
void render();
void loadTexture();

// Global variables
GLFWwindow *mainWindow = NULL;
Shader *globalShader = NULL;
Shader *tableShader = NULL;
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;
table_t *table;
myblock_t* myblock;
glm::mat4 projection, view, model;

// for arcball
float arcballSpeed = 0.2f;
static rotor_t camRotor(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true );
static rotor_t modelRotor(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

// for texture
static unsigned int texture; // Array of texture ids.


int main()
{
    mainWindow = glAllInit();
    
    // shader loading and compile (by calling the constructor)
    globalShader = new Shader("shader/global.vs", "shader/global.fs");
    tableShader = new Shader("shader/table.vs", "shader/table.fs");
    
    // projection matrix
    globalShader->use();
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    globalShader->setMat4("projection", projection);

    tableShader->use();
    tableShader->setMat4("projection", projection);

    loadTexture();
    
    // Table initialization.
    table = new table_t();
    
    // My block initialization.
    myblock = new myblock_t(GRID_START_X, GRID_START_Y, TABLE_HEIGHT + CUBE_HALF_EDGE);

    cout << "ARCBALL: camera rotation mode" << endl;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(mainWindow)) {
        render();
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

GLFWwindow *glAllInit()
{
    GLFWwindow *window;
    
    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "rotor_t", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    // OpenGL states
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    
    return window;
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    view = glm::lookAt(glm::vec3(sin(glm::radians(60.0f)) * 7.0f, 0.0f, cos(glm::radians(60.0f)) * 7.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(-cos(glm::radians(30.0f)), 0.0f, sin(glm::radians(30.0f))));
    view = view * camRotor.createRotationMatrix();
    
    globalShader->use();
    globalShader->setMat4("view", view);
    tableShader->use();
    tableShader->setMat4("view", view);
    
    // My block.
    globalShader->use();
    model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.65f));
    globalShader->setMat4("model", model);
    myblock->draw(globalShader);
    
    // Table
    tableShader->use();
    model = glm::mat4(1.0f);
    model = model * modelRotor.createRotationMatrix();
    tableShader->setMat4("model", model);
    glBindTexture(GL_TEXTURE_2D, texture);
    table->draw(tableShader);

    // right cube
    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    //globalShader->setMat4("model", model);
    //cube->draw(globalShader);
    
    glfwSwapBuffers(mainWindow);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height; 
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camRotor.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelRotor.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: model  rotation mode" << endl;
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (arcballCamRot)
        camRotor.mouseButtonCallback( window, button, action, mods );
    else
        modelRotor.mouseButtonCallback( window, button, action, mods );
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (arcballCamRot)
        camRotor.cursorCallback( window, x, y );
    else
        modelRotor.cursorCallback( window, x, y );
}

void loadTexture() {

    // Create texture ids.
    glGenTextures(1, &texture);

    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters for wrapping.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the image from bmp file
    BitMapFile *image;
    char texFileName[] = "img/grid.bmp";
    if (!(image = getbmp(texFileName))) {
        cout << texFileName << " open error ... exit" << endl;
        exit(1);
    }
    else {
        cout << texFileName << " loaded. size: " << image->sizeX << " x " << image->sizeY << endl;
    }

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    glGenerateMipmap(GL_TEXTURE_2D);

}


