//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "glm/glm.hpp"
#include "camera.h"
#include "CameraController.h"
#include "Engine/Mesh.h"
#include "Engine/Material.h"

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
    GLuint  v_buffer_handle;
    Camera *camera_;
    glm::mat4 modelMat;
    std::vector<GLushort> indices;
    CameraControler *controler_;

    void framebuffer_resize_callback(int w, int h) override;

    void cursor_position_callback(double x, double y);

    void mouse_button_callback(int button, int action, int mods);

    std::vector<xe::Mesh*> meshes_;
    void add_submesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }
};