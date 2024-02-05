#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"

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


#include "Application/utils.h"
#include "Engine/Material.h"
#include "Engine/mesh_loader.h"


void SimpleShapeApplication::init() {

    set_camera(new Camera);

    set_controler(new CameraControler(camera()));

    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    xe::ColorMaterial::init();


    add_submesh(xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj",
                                     std::string(ROOT_DIR) + "/Models"));

    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, v_buffer_handle);

    
    modelMat = glm::mat4(1.0f);

    auto [w, h] = frame_buffer_size();

    auto fov = glm::pi<float>() / 4.0;
    auto _near = 0.1f;
    auto _far = 100.0f;
    auto aspect = (float) w / h;


    camera_->perspective(fov, aspect, _near, _far);

    camera_->look_at(
        glm::vec3(2.0f, -1.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"
    
    modelMat = glm::mat4(1.0f);
    modelMat = glm::scale(modelMat, glm::vec3{1.0f, 1.0f, 1.0f});
    modelMat = glm::rotate(modelMat, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::translate(modelMat, glm::vec3{0.0f, 0.0f, 0.0f});


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
    glBindBuffer(GL_UNIFORM_BUFFER, v_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), &pvm);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


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