struct Target {
	GLuint vao;
	GLuint vbo;

	GLuint framebuffer;
	GLuint tex;
	GLuint renderbuffer;
};

bool target_init_shader();

void target_init(Target *target, int width, int height);

void target_render(Target target);

void target_free_shader();

void target_free(Target target);
