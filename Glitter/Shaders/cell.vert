#version 450 core
layout (location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * mat4(model) * vec4(vertex.xy, 0.0, 1.0);
}