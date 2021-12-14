#version 450 core
out vec4 color;

uniform vec3 cell_color;

void main() {
    color = vec4(cell_color, 1.0);
}
