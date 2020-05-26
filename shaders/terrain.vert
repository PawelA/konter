#version 330 core

layout(location = 0) in vec3 in_coords;
layout(location = 1) in vec2 in_color;

flat out vec2 color;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(in_coords, 1.0);
    color = in_color;
}

