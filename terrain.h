struct Terrain {
	GLuint vao;
	GLuint vbo;
	int index_cnt;
};

bool terrain_init_shader();

void terrain_init(Terrain *terrain, Mesh mesh);

void terrain_render(Terrain terrain, Player player, float aspect_ratio);

void terrain_free_shader();

void terrain_free(Terrain terrain);
