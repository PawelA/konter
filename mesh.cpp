#include <stdio.h>
#include <vector>
#include <map>

#include "vec.h"
#include "mesh.h"

bool mesh_load(Mesh *mesh, const char *name) {
	FILE *stream = fopen(name, "r");
	if (!stream) {
		printf("%s: no such file\n", name);
		return false;
	}

	// assume no monstrous lines breaking our parser for now
	// TODO: do something about this
	const int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];

	std::vector<vec3> vert_vec;
	std::vector<int> index_vec;
	
	float f1, f2, f3;
	int   i1, i2, i3;

	while (fgets(buff, BUFF_SIZE, stream)) {
		// vertex
		if (sscanf(buff, "v %f %f %f",  &f1, &f2, &f3) == 3)
			vert_vec.push_back(make_vec3(f1, f2, f3));
		// face
		else if (sscanf(buff, "f %i %i %i",  &i1, &i2, &i3) == 3) {
			index_vec.push_back(i1-1);
			index_vec.push_back(i2-1);
			index_vec.push_back(i3-1);
		}
		// face with vertex normals (just ignore them)
		else if (sscanf(buff, "f %i//%*i %i//%*i %i//%*i", &i1, &i2, &i3) == 3) {
			index_vec.push_back(i1-1);
			index_vec.push_back(i2-1);
			index_vec.push_back(i3-1);
		}
	}

	fclose(stream);

	mesh->verts = new vec3[vert_vec.size()];
	mesh->vert_cnt = vert_vec.size();

	mesh->indices = new int[index_vec.size()];
	mesh->index_cnt = index_vec.size();

	for (int i = 0; i < (int)vert_vec.size(); i++)
		mesh->verts[i] = vert_vec[i];

	for (int i = 0; i < (int)index_vec.size(); i++)
		mesh->indices[i] = index_vec[i];

	return true;
}

void mesh_perturb(Mesh *mesh, float val) {
	for (int i = 0; i < mesh->vert_cnt; i++) {
		mesh->verts[i].x += val * rand() / RAND_MAX;
		mesh->verts[i].y += val * rand() / RAND_MAX;
		mesh->verts[i].z += val * rand() / RAND_MAX;
	}
}

inline float det2(vec2 u, vec2 v) {
	return u.x * v.y - u.y * v.x;
}

inline int sgnf(float x) {
	return x > 0.0f ? 1 : (x < 0.0f ? -1 : 0);
}

inline vec2 projx(vec3 v) {
	return make_vec2(v.y, v.z);
}

inline vec2 projy(vec3 v) {
	return make_vec2(v.x, v.z);
}

inline vec2 projz(vec3 v) {
	return make_vec2(v.x, v.y);
}

void mesh_expand(Mesh mesh, Mesh *mesh_out, float x, float y, float z) {
	mesh_out->verts = new vec3[8 * mesh.vert_cnt];
	mesh_out->vert_cnt = 8 * mesh.vert_cnt;

	for (int i = 0; i < mesh.vert_cnt; i++) {
		mesh_out->verts[8 * i + 0] = mesh.verts[i] + make_vec3(+x, +y, +z);
		mesh_out->verts[8 * i + 1] = mesh.verts[i] + make_vec3(+x, +y, -z);
		mesh_out->verts[8 * i + 2] = mesh.verts[i] + make_vec3(+x, -y, +z);
		mesh_out->verts[8 * i + 3] = mesh.verts[i] + make_vec3(+x, -y, -z);
		mesh_out->verts[8 * i + 4] = mesh.verts[i] + make_vec3(-x, +y, +z);
		mesh_out->verts[8 * i + 5] = mesh.verts[i] + make_vec3(-x, +y, -z);
		mesh_out->verts[8 * i + 6] = mesh.verts[i] + make_vec3(-x, -y, +z);
		mesh_out->verts[8 * i + 7] = mesh.verts[i] + make_vec3(-x, -y, -z);
	}
	std::vector<int> indices;

	// faces
	for (int i = 0; i < mesh.index_cnt; i += 3) {
		vec3 t1 = mesh.verts[mesh.indices[i]];
		vec3 t2 = mesh.verts[mesh.indices[i + 1]];
		vec3 t3 = mesh.verts[mesh.indices[i + 2]];
		vec3 n = cross(t2 - t1, t3 - t1);

		if (n.x != 0.0f && n.y != 0.0f && n.z != 0.0f) {
			int a = 4 * (n.x < 0) + 2 * (n.y < 0) + (n.z < 0);
			indices.push_back(8 * mesh.indices[i] + a);
			indices.push_back(8 * mesh.indices[i + 1] + a);
			indices.push_back(8 * mesh.indices[i + 2] + a);
		}
	}

	// verts
	for (int i = 0; i < mesh.vert_cnt; i++) {
		vec3 v = mesh.verts[i];

		bool xpos = false, xneg = false;
		bool ypos = false, yneg = false;
		bool zpos = false, zneg = false;

		for (int j = 0; j < mesh.index_cnt; j++) {
			if (mesh.indices[j] == i) {
				int j1 = (j % 3 == 0 ? j + 1 : j - j % 3);
				int j2 = (j % 3 == 2 ? j - 1 : j - j % 3 + 2);
				vec3 d1 = mesh.verts[mesh.indices[j1]] - v;
				vec3 d2 = mesh.verts[mesh.indices[j2]] - v;

				if (d1.x > 0.0f || d2.x > 0.0f) xpos = true;
				if (d1.x < 0.0f || d2.x < 0.0f) xneg = true;

				if (d1.y > 0.0f || d2.y > 0.0f) ypos = true;
				if (d1.y < 0.0f || d2.y < 0.0f) yneg = true;

				if (d1.z > 0.0f || d2.z > 0.0f) zpos = true;
				if (d1.z < 0.0f || d2.z < 0.0f) zneg = true;
			}
		}

		// TODO: stop if vertex is concave

		if (!xpos) {
			indices.push_back(8 * i + 0);
			indices.push_back(8 * i + 2);
			indices.push_back(8 * i + 1);
			indices.push_back(8 * i + 3);
			indices.push_back(8 * i + 1);
			indices.push_back(8 * i + 2);
		}
		if (!xneg) {
			indices.push_back(8 * i + 4);
			indices.push_back(8 * i + 5);
			indices.push_back(8 * i + 6);
			indices.push_back(8 * i + 7);
			indices.push_back(8 * i + 6);
			indices.push_back(8 * i + 5);
		}
		if (!ypos) {
			indices.push_back(8 * i + 0);
			indices.push_back(8 * i + 1);
			indices.push_back(8 * i + 4);
			indices.push_back(8 * i + 5);
			indices.push_back(8 * i + 4);
			indices.push_back(8 * i + 1);
		}
		if (!yneg) {
			indices.push_back(8 * i + 2);
			indices.push_back(8 * i + 6);
			indices.push_back(8 * i + 3);
			indices.push_back(8 * i + 7);
			indices.push_back(8 * i + 3);
			indices.push_back(8 * i + 6);
		}
		if (!zpos) {
			indices.push_back(8 * i + 0);
			indices.push_back(8 * i + 4);
			indices.push_back(8 * i + 2);
			indices.push_back(8 * i + 6);
			indices.push_back(8 * i + 2);
			indices.push_back(8 * i + 4);
		}
		if (!zneg) {
			indices.push_back(8 * i + 1);
			indices.push_back(8 * i + 3);
			indices.push_back(8 * i + 5);
			indices.push_back(8 * i + 7);
			indices.push_back(8 * i + 5);
			indices.push_back(8 * i + 3);
		}
	}

	// edges
	std::map<std::pair<int, int>, int> opposite_vert;
	for (int i = 0; i < mesh.index_cnt; i += 3) {
		int i0 = mesh.indices[i];
		int i1 = mesh.indices[i + 1];
		int i2 = mesh.indices[i + 2];
		opposite_vert[std::make_pair(i0, i1)] = i2;
		opposite_vert[std::make_pair(i1, i2)] = i0;
		opposite_vert[std::make_pair(i2, i0)] = i1;
	}

	for (auto p : opposite_vert) {
		int i1 = p.first.first;
		int i2 = p.first.second;
		int i3 = p.second;

		if (i1 > i2 || opposite_vert.find(std::make_pair(i2, i1)) == opposite_vert.end())
			continue;

		vec3 v1 = mesh.verts[i1];
		vec3 v2 = mesh.verts[i2];
		vec3 u1 = mesh.verts[i3];
		vec3 u2 = mesh.verts[opposite_vert[std::make_pair(i2, i1)]];

		// TODO: stop if edge is concave
		
		if (v1.x != v2.x && v1.y != v2.y && v1.z != v2.z) {
			int d1, d2;
			int e1, e2;

			// x
			d1 = sgnf(det2(projx(v1)-projx(u1), projx(v2)-projx(u1)));
			d2 = sgnf(det2(projx(v1)-projx(u2), projx(v2)-projx(u2)));
			if (d1 * d2 > 0) {
				if (v1.y > v2.y && v1.z > v2.z && d1 > 0) {
					e1 = 2;
					e2 = 6;
				}
				else if (v1.y > v2.y && v1.z > v2.z && d1 < 0) {
					e1 = 5;
					e2 = 1;
				}
				else if (v1.y > v2.y && v1.z < v2.z && d1 > 0) {
					e1 = 0;
					e2 = 4;
				}
				else if (v1.y > v2.y && v1.z < v2.z && d1 < 0) {
					e1 = 7;
					e2 = 3;
				}
				else if (v1.y < v2.y && v1.z > v2.z && d1 > 0) {
					e1 = 3;
					e2 = 7;
				}
				else if (v1.y < v2.y && v1.z > v2.z && d1 < 0) {
					e1 = 4;
					e2 = 0;
				}
				else if (v1.y < v2.y && v1.z < v2.z && d1 > 0) {
					e1 = 1;
					e2 = 5;
				}
				else if (v1.y < v2.y && v1.z < v2.z && d1 < 0) {
					e1 = 6;
					e2 = 2;
				}
				// TODO
				else
					return;

				indices.push_back(8 * i1 + e1);
				indices.push_back(8 * i1 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e1);
				indices.push_back(8 * i1 + e1);
			}

			// y
			d1 = sgnf(det2(projy(v1)-projy(u1), projy(v2)-projy(u1)));
			d2 = sgnf(det2(projy(v1)-projy(u2), projy(v2)-projy(u2)));
			if (d1 * d2 > 0) {
				if (v1.x > v2.x && v1.z > v2.z && d1 > 0) {
					e1 = 6;
					e2 = 4;
				}
				else if (v1.x > v2.x && v1.z > v2.z && d1 < 0) {
					e1 = 1;
					e2 = 3;
				}
				else if (v1.x > v2.x && v1.z < v2.z && d1 > 0) {
					e1 = 2;
					e2 = 0;
				}
				else if (v1.x > v2.x && v1.z < v2.z && d1 < 0) {
					e1 = 5;
					e2 = 7;
				}
				else if (v1.x < v2.x && v1.z > v2.z && d1 > 0) {
					e1 = 7;
					e2 = 5;
				}
				else if (v1.x < v2.x && v1.z > v2.z && d1 < 0) {
					e1 = 0;
					e2 = 2;
				}
				else if (v1.x < v2.x && v1.z < v2.z && d1 > 0) {
					e1 = 3;
					e2 = 1;
				}
				else if (v1.x < v2.x && v1.z < v2.z && d1 < 0) {
					e1 = 4;
					e2 = 6;
				}

				indices.push_back(8 * i1 + e1);
				indices.push_back(8 * i1 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e1);
				indices.push_back(8 * i1 + e1);
			}

			// z
			d1 = sgnf(det2(projz(v1)-projz(u1), projz(v2)-projz(u1)));
			d2 = sgnf(det2(projz(v1)-projz(u2), projz(v2)-projz(u2)));
			if (d1 * d2 > 0) {
				if (v1.x > v2.x && v1.y > v2.y && d1 > 0) {
					e1 = 4;
					e2 = 5;
				}
				else if (v1.x > v2.x && v1.y > v2.y && d1 < 0) {
					e1 = 3;
					e2 = 2;
				}
				else if (v1.x > v2.x && v1.y < v2.y && d1 > 0) {
					e1 = 0;
					e2 = 1;
				}
				else if (v1.x > v2.x && v1.y < v2.y && d1 < 0) {
					e1 = 7;
					e2 = 6;
				}
				else if (v1.x < v2.x && v1.y > v2.y && d1 > 0) {
					e1 = 6;
					e2 = 7;
				}
				else if (v1.x < v2.x && v1.y > v2.y && d1 < 0) {
					e1 = 1;
					e2 = 0;
				}
				else if (v1.x < v2.x && v1.y < v2.y && d1 > 0) {
					e1 = 2;
					e2 = 3;
				}
				else if (v1.x < v2.x && v1.y < v2.y && d1 < 0) {
					e1 = 5;
					e2 = 4;
				}

				indices.push_back(8 * i1 + e1);
				indices.push_back(8 * i1 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e2);
				indices.push_back(8 * i2 + e1);
				indices.push_back(8 * i1 + e1);
			}
		}
	}

	mesh_out->indices = new int[indices.size()];
	mesh_out->index_cnt = indices.size();

	for (int i = 0; i < (int)indices.size(); i++)
		mesh_out->indices[i] = indices[i];
}

void mesh_free(Mesh mesh) {
	delete[] mesh.verts;
	delete[] mesh.indices;
}
