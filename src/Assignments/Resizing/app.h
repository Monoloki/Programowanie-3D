//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "camera.h"

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

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

private:
    GLuint vao_, u_vertex_buffer_handle, v_buffer_handle, u_fragment_buffer_handle;
    Camera *camera_;
    glm::mat4 modelMat;

    void framebuffer_resize_callback(int w, int h) override;
};