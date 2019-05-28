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
#include <vector>

#include <shader.h>
#include <getbmp.h>

#include "def.h"
#include "block.h"
#include "table.h"
#include "rotor.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
void render();
void loadTableTexture();
unsigned int loadTexture(char*);


// Global variables
GLFWwindow *mainWindow = NULL;
Shader *globalShader = NULL;
Shader *tableShader = NULL;
Shader *lineShader = NULL;
unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;
table_t *table;
block_t* my_block;
glm::mat4 projection, view, model;
vector<block_t*> blocks;
glm::vec3 cameraPos = glm::vec3(sin(glm::radians(60.0f)) * 7.0f, 0.0f, cos(glm::radians(60.0f)) * 7.0f);

// for arcball
float arcballSpeed = 0.2f;
static rotor_t camRotor(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true );
//static rotor_t modelRotor(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
//bool arcballCamRot = true;

// for texture
static unsigned int table_texture, cube_texture, cube_specular_texture; // Array of texture ids.

void print_help() {
    cout << "ConnectBlocks: Make a square!" << endl;
    cout << "Move: ← → ↑ ↓\nBind: Space" << endl;
}


int main()
{
    print_help();

    mainWindow = glAllInit();
    
    // shader loading and compile (by calling the constructor)
    //globalShader = new Shader("shader/global.vs", "shader/global.fs");
    //tableShader = new Shader("shader/table.vs", "shader/table.fs");

    globalShader = new Shader("shader/lighting.vs", "shader/lighting.fs");
    tableShader = new Shader("shader/table.vs", "shader/table.fs");
    lineShader = new Shader("shader/lines.vs", "shader/lines.fs");
    
    // projection matrix
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    globalShader->use();
    globalShader->setMat4("projection", projection);
    globalShader->setInt("material.diffuse", 0);
    globalShader->setInt("material.specular", 1);
    globalShader->setFloat("material.shininess", 32);

    globalShader->setVec3("viewPos", cameraPos);

    // directional light
    globalShader->setVec3("dirLight.direction", -0.0f, -0.0f, -1.0f);
    globalShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    globalShader->setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
    globalShader->setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);


    tableShader->use();
    tableShader->setMat4("projection", projection);

    lineShader->use();
    lineShader->setMat4("projection", projection);

    // load texture
    loadTableTexture();
    cube_texture = loadTexture("img/container2.bmp");
    cube_specular_texture = loadTexture("img/container2_specular.bmp");
    
    // Table initialization.
    table = new table_t();
    
    // Initialize blocks.
    block_t::init_random_placement(/*num_blocks*/9);
    
    // Choose random block as starting block.
    my_block = block_t::get_starting_block();

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
    lineShader->use();
    lineShader->setMat4("view", view);
    
    // Blocks
    globalShader->use();
    model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.65f));
    globalShader->setMat4("model", model);

    lineShader->use();
    model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.65f));
    lineShader->setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cube_specular_texture);
    my_block->draw(globalShader, lineShader);
    
    // Table
    tableShader->use();
    model = glm::mat4(1.0f);
    tableShader->setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, table_texture);
    table->draw(globalShader);

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
        if(block_t::game_done) {
            my_block = block_t::restart_game();
        }
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        my_block->move_right();
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        my_block->move_left();
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        my_block->move_up();
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        my_block->move_down();
    }
    else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        my_block->bind();
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    camRotor.mouseButtonCallback( window, button, action, mods );
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    camRotor.cursorCallback( window, x, y );
}

void loadTableTexture() {

    // Create texture ids.
    glGenTextures(1, &table_texture);

    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, table_texture);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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

unsigned int loadTexture(char *texFileName) {
    unsigned int texture;

    // Create texture ids.
    glGenTextures(1, &texture);

    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters for wrapping.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
    unsigned char *image = stbi_load(texFileName, &width, &height, &nrChannels, 0);
    if (!image) {
        printf("texture %s loading error ... \n", texFileName);
    }
    else printf("texture %s loaded\n", texFileName);

    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;

    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}


