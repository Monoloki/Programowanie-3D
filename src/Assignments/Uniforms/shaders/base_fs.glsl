#version 410

layout(location=0) out vec4 vFragColor;
layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color; 
};

in vec4 Color;

void main() {
    vFragColor = Color;
}
