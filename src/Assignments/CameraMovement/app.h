//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "camera.h"
#include "CameraController.h"

class SimpleShapeApplication : public xe::Application
{
public:   
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);   
        camera()->zoom(yoffset / 30.0f);
    }

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }

    void set_controler(CameraControler *controler) { controler_ = controler; }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

private:
    GLuint vao_, u_vertex_buffer_handle, v_buffer_handle, u_fragment_buffer_handle;
    Camera *camera_;
    glm::mat4 modelMat;
    CameraControler *controler_;

    void framebuffer_resize_callback(int w, int h) override;

    void cursor_position_callback(double x, double y);

    void mouse_button_callback(int button, int action, int mods);
};