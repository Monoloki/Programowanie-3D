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

    set_controler(new CameraControler(camera()));

    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    xe::ColorMaterial::init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
         0.5f, 0.0f, -0.5f,  
        -0.5f, 0.0f, -0.5f, 
        -0.5f, 0.0f, 0.5f,  

         0.5f, 0.0f, -0.5f, 
        -0.5f, 0.0f, 0.5f,  
         0.5f, 0.0f, 0.5f,   

         0.5f, 0.0f, -0.5f, 
         0.0f, 1.0f, 0.0f,  
        -0.5f, 0.0f, -0.5f, 

        -0.5f, 0.0f, -0.5f,  
         0.0f, 1.0f, 0.0f,   
        -0.5f, 0.0f, 0.5f, 

        -0.5f, 0.0f, 0.5f,  
         0.0f, 1.0f, 0.0f,  
         0.5f, 0.0f, 0.5f,  

         0.5f, 0.0f, 0.5f,   
         0.0f, 1.0f, 0.0f,  
         0.5f, 0.0f, -0.5f, 

    };

    indices = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
    };

    auto pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

    pyramid->allocate_index_buffer(indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_indices(0, indices.size() * sizeof(GLfloat), indices.data());

    pyramid->add_submesh(0, 3, new xe::ColorMaterial({0.5, 0.0, 0.0, 1.0f}) );
    pyramid->add_submesh(3, 6, new xe::ColorMaterial({1.0, 0.0, 0.0, 1.0f}) );
    pyramid->add_submesh(6, 9, new xe::ColorMaterial({0.0, 1.0, 0.0, 1.0f}) );
    pyramid->add_submesh(9, 12, new xe::ColorMaterial({ 0.0, 0.0, 1.0, 1.0f}) );
    pyramid->add_submesh(12, 15, new xe::ColorMaterial({1.0, 1.0, 1.0, 1.0f}) );
    pyramid->add_submesh(15, 18, new xe::ColorMaterial({1.0, 0.0, 1.0, 1.0f}) );  
    add_submesh(pyramid);



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

    auto [w, h] = frame_buffer_size();

    auto fov = glm::pi<float>() / 4.0;
    auto near = 0.1f;
    auto far = 100.0f;
    auto aspect = (float) w / h;


    camera_->perspective(fov, aspect, near, far);

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
    
    glViewport(0, 0, w, h);

    glUseProgram(program);

}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    auto pvm = camera_->projection() * camera_->view() * modelMat;
    glBindBuffer(GL_UNIFORM_BUFFER, u_vertex_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), &pvm);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 18);
    //glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

    for (auto m: meshes_)
        m->draw();

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera_->set_aspect((float) w / h);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}