#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include "Application/utils.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void SimpleShapeApplication::init() {

    set_camera(new Camera);

    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
         0.5f, 0.0f, -0.5f,  0.5, 0.0, 0.0,
        -0.5f, 0.0f, -0.5f,  0.5, 0.0, 0.0,
        -0.5f, 0.0f, 0.5f,   0.5, 0.0, 0.0,

         0.5f, 0.0f, -0.5f,  1.0, 0.0, 0.0,
        -0.5f, 0.0f, 0.5f,   1.0, 0.0, 0.0,
         0.5f, 0.0f, 0.5f,   1.0, 0.0, 0.0,

         0.5f, 0.0f, -0.5f,  0.0, 1.0, 0.0,
         0.0f, 1.0f, 0.0f,   0.0, 1.0, 0.0,
        -0.5f, 0.0f, -0.5f,  0.0, 1.0, 0.0,

        -0.5f, 0.0f, -0.5f,  0.0, 0.0, 1.0,
         0.0f, 1.0f, 0.0f,   0.0, 0.0, 1.0,
        -0.5f, 0.0f, 0.5f,   0.0, 0.0, 1.0,

        -0.5f, 0.0f, 0.5f,   1.0, 1.0, 0.0,
         0.0f, 1.0f, 0.0f,   1.0, 1.0, 0.0,
         0.5f, 0.0f, 0.5f,   1.0, 1.0, 0.0,

         0.5f, 0.0f, 0.5f,   1.0, 0.0, 1.0,
         0.0f, 1.0f, 0.0f,   1.0, 0.0, 1.0,
         0.5f, 0.0f, -0.5f,  1.0, 0.0, 1.0

    };
  

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle, u_fragment_buffer_handle;

    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   


    // Creating and binding the buffer for modifying pixel color
    glGenBuffers(1, &u_fragment_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, u_fragment_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_fragment_buffer_handle);

    glGenBuffers(1, &u_vertex_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, u_vertex_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_vertex_buffer_handle);
    
    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    
    //matrix model
    modelMat = glm::mat4(1.0f);

    camera_->perspective(1.0f, 1.0f, 1.0f, 5.0f);

    camera_->look_at(
        glm::vec3(2.0f, -1.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);

}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    auto pvm = camera_->projection() * camera_->view() * modelMat;
    glBindBuffer(GL_UNIFORM_BUFFER, u_vertex_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), &pvm);
    
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 18);
    //glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    
}
