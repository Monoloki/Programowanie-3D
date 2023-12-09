#version 460

layout(location=0) in  vec4 vertex_position;
layout(location=1) in  vec3 vertex_color;

out vec3 in_color;

layout(std140, binding=1) uniform Transformations {
 vec2 scale;
 vec2 translation;
 mat2 rotation;
 };

void main() {
    in_color = vertex_color;
    gl_Position.xy = rotation * (scale * vertex_position.xy) + translation;
    gl_Position.zw = vertex_position.zw;
}