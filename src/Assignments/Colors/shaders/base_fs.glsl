#version 410

layout(location=0) out vec4 vFragColor;

in vec4 Color;

void main() {
    vFragColor = Color;
}
