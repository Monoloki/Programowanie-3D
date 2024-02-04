#version 460

layout(location=0) out vec4 vFragColor;

in vec3 in_color;

layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color;
};

void main() {
    vFragColor = vec4(in_color, 1.0f);
}

