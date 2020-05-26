#include <stddef.h>

#include "gl.h"
#include "shader.h"
#include "target.h"

static GLuint prog;

static const GLfloat quad_data[] = {
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f,  1.0f,
};

bool target_init_shader() {
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

	if (!compile_shader(vert, "shaders/tex.vert")) return false;
	if (!compile_shader(frag, "shaders/tex.frag")) return false;

	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	if (!link_program(prog)) return false;

	glDetachShader(prog, vert);
	glDetachShader(prog, frag);
	glDeleteShader(vert);
	glDeleteShader(frag);

	return true;
}

void target_init(Target *target, int width, int height) {
	// Generate quad vao and vbo
	glGenVertexArrays(1, &target->vao);
	glBindVertexArray(target->vao);
	glGenBuffers(1, &target->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, target->vbo);
	// Write quad data
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Generate framebuffer
	glGenFramebuffers(1, &target->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);

	// Generate texture
	glGenTextures(1, &target->tex);
	glBindTexture(GL_TEXTURE_2D, target->tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Attach it to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target->tex, 0);

	// Generate depth renderbuffer
	glGenRenderbuffers(1, &target->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, target->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// Attach it to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, target->renderbuffer);

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void target_render(Target target) {
	glUseProgram(prog);
	glBindVertexArray(target.vao);
	glBindTexture(GL_TEXTURE_2D, target.tex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void target_free_shader() {
	glDeleteProgram(prog);
}

void target_free(Target target) {
	glDeleteVertexArrays(1, &target.vao);
	glDeleteBuffers(1, &target.vbo);
	glDeleteFramebuffers(1, &target.framebuffer);
	glDeleteTextures(1, &target.tex);
	glDeleteRenderbuffers(1, &target.renderbuffer);
}
