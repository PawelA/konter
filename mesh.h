struct Mesh {
	vec3 *verts;
	int vert_cnt;

	int *indices;
	int index_cnt;
};

bool mesh_load(Mesh *mesh, const char *name);

void mesh_perturb(Mesh *mesh, float val);

void mesh_expand(Mesh mesh, Mesh *mesh_out, float x, float y, float z);

void mesh_free(Mesh mesh);
