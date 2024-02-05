#version 460

layout(location=0) in  vec4 vertex_position;


layout(std140, binding=1) uniform Transformations {
 mat4 PVM;
 };

void main() {
    gl_Position =  PVM * vertex_position;
}


