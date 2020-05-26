#version 330 core

layout(location = 0) in vec2 pos;
out vec2 tex_pos;

void main() {
    tex_pos = 0.5f * (pos + 1.0f);
    gl_Position = vec4(pos, 0.0f, 1.0f);
}
