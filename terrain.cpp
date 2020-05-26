#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <queue>

#include "gl.h"
#include "shader.h"

#include "vec.h"
#include "mat.h"
#include "mat_util.h"
#include "constants.h"

#include "mesh.h"
#include "player.h"
#include "terrain.h"

static GLuint prog;
static GLint mvp_location;

struct Graph {
	int *data;
	int *deg;
	int **adj;
	int vert_cnt;
};

struct Half_edge {
	int from;
	int to;
	int face;
};

static Half_edge make_half_edge(int from, int to, int face) {
	Half_edge h;

	if (from < to) {
		h.from = from;
		h.to = to;
	}
	else {
		h.from = to;
		h.to = from;
	}
	h.face = face;

	return h;
}

static int half_edge_cmp(const void *a1, const void *a2) {
	Half_edge h1 = *((Half_edge *) a1);
	Half_edge h2 = *((Half_edge *) a2);

	if (h1.from < h2.from) return -1;
	if (h1.from > h2.from) return +1;
	if (h1.to < h2.to) return -1;
	if (h1.to > h2.to) return +1;

	return 0;
}

static void del_edge(Graph graph, int u, int v) {
	for (int i = 0; i < graph.deg[u]; i++) {
		if (graph.adj[u][i] == v) {
			graph.deg[u]--;
			graph.adj[u][i] = graph.adj[u][graph.deg[u]];
		}
	}
	for (int i = 0; i < graph.deg[v]; i++) {
		if (graph.adj[v][i] == u) {
			graph.deg[v]--;
			graph.adj[v][i] = graph.adj[v][graph.deg[v]];
		}
	}
}

static bool make_graph(Mesh mesh, Graph *graph) {
	Half_edge *h_edges;
	int vert_cnt = 0;
	int edge_cnt = 0;

	h_edges = (Half_edge *) malloc(mesh.index_cnt * sizeof(Half_edge));
	vert_cnt = mesh.index_cnt / 3;
	graph->vert_cnt = vert_cnt;
	graph->deg = (int *) calloc(vert_cnt, sizeof(int));
	graph->adj = (int **) malloc(vert_cnt * sizeof(int *));

	for (int i = 0; i < mesh.index_cnt; i += 3) {
		h_edges[i+0] = make_half_edge(mesh.indices[i+0], mesh.indices[i+1], i / 3);
		h_edges[i+1] = make_half_edge(mesh.indices[i+1], mesh.indices[i+2], i / 3);
		h_edges[i+2] = make_half_edge(mesh.indices[i+2], mesh.indices[i+0], i / 3);
	}

	qsort(h_edges, mesh.index_cnt, sizeof(Half_edge), half_edge_cmp);

	for (int i = 0; i < mesh.index_cnt - 1; i++) {
		if (half_edge_cmp(&h_edges[i], &h_edges[i+1]) == 0) {
			int u = h_edges[i].face;
			int v = h_edges[i+1].face;
			graph->deg[u]++;
			graph->deg[v]++;
			edge_cnt++;
		}
	}

	graph->data = (int *) malloc(2 * edge_cnt * sizeof(int));

	graph->adj[0] = graph->data + graph->deg[0];
	for (int i = 1; i < vert_cnt; i++)
		graph->adj[i] = graph->adj[i-1] + graph->deg[i];

	for (int i = 0; i < mesh.index_cnt - 1; i++) {
		if (half_edge_cmp(&h_edges[i], &h_edges[i+1]) == 0) {
			int u = h_edges[i].face;
			int v = h_edges[i+1].face;
			graph->adj[u]--;
			graph->adj[u][0] = v;
			graph->adj[v]--;
			graph->adj[v][0] = u;
		}

	}

	free(h_edges);

	return true;
}

static void path(Graph graph, int u, int *next) {
	int v;

	while (graph.deg[u] == 1) {
		v = graph.adj[u][0];
		del_edge(graph, u, v);
		next[u] = v;
		u = v;
	}
}

static void direct(Graph graph, int *matched, int *next) {
	std::queue<int> unm;

	for (int i = 0; i < graph.vert_cnt; i++) {
		matched[i] = -1;
		next[i] = -1;
		if (graph.deg[i] == 3)
			unm.push(i);
	}

	while (!unm.empty()) {
		int v = unm.front();
		unm.pop();

		if (matched[v] != -1) continue;

		int u = -1;
		if (matched[graph.adj[v][0]] == -1) u = graph.adj[v][0];
		if (matched[graph.adj[v][1]] == -1) u = graph.adj[v][1];
		if (matched[graph.adj[v][2]] == -1) u = graph.adj[v][2];
		
		if (u == -1) {
			u = graph.adj[v][rand() % 3];
			matched[matched[u]] = -1;
			if (graph.deg[matched[u]] == 3)
				unm.push(matched[u]);
		}

		matched[v] = u;
		matched[u] = v;
	}

	for (int i = 0; i < graph.vert_cnt; i++) {
		if (i < matched[i])
			del_edge(graph, i, matched[i]);
	}

	for (int i = 0; i < graph.vert_cnt; i++) {
		if (graph.deg[i] == 1) {
			path(graph, i, next);
		}
	}
	
	for (int i = 0; i < graph.vert_cnt; i++) {
		if (graph.deg[i] == 2) {
			int j = graph.adj[i][0];
			next[j] = i;
			del_edge(graph, i, j);
			path(graph, i, next);
		}
	}
}


static float hash(int x) {
	return (abs(42069 + x * 1000000007) % 65535) / 65536.0f;
}

static void write_data(Mesh mesh, GLfloat *data) {
	Graph graph;

	for (int i = 0; i < mesh.index_cnt; i++) {
		data[i * 5 + 0] = mesh.verts[mesh.indices[i]].x;
		data[i * 5 + 1] = mesh.verts[mesh.indices[i]].y;
		data[i * 5 + 2] = mesh.verts[mesh.indices[i]].z;
	}

	make_graph(mesh, &graph);

	int *matched = (int *) malloc(2 * graph.vert_cnt * sizeof(int));
	int *next = matched + graph.vert_cnt;

	direct(graph, matched, next);

	for (int i = 0; i < mesh.index_cnt; i++) {
		data[i * 5 + 3] = hash(std::min(i/3, matched[i/3]));
		data[i * 5 + 4] = hash(std::min(next[i/3], matched[next[i/3]]));
	}

	free(matched);
}

bool terrain_init_shader() {
	GLuint vert;
	GLuint frag;

	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	if (!compile_shader(vert, "shaders/terrain.vert")) return false;
	if (!compile_shader(frag, "shaders/terrain.frag")) return false;

	prog = glCreateProgram();

	glAttachShader(prog, vert);
	glAttachShader(prog, frag);

	if (!link_program(prog)) return false;

	glDetachShader(prog, vert);
	glDetachShader(prog, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	mvp_location = glGetUniformLocation(prog, "mvp");

	return true;
}

void terrain_init(Terrain *terrain, Mesh mesh) {
	int data_len = mesh.index_cnt * 5;
	GLfloat *data = (GLfloat *) malloc(data_len * sizeof(GLfloat));
	write_data(mesh, data);
	
	glGenVertexArrays(1, &terrain->vao);
	glBindVertexArray(terrain->vao);
	
	glGenBuffers(1, &terrain->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, terrain->vbo);
	glBufferData(GL_ARRAY_BUFFER, data_len * sizeof(GLfloat), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)(sizeof(GLfloat) * 3));

	terrain->index_cnt = mesh.index_cnt;
}

void terrain_render(Terrain terrain, Player player, float aspect_ratio) {
	glUseProgram(prog);
	glBindVertexArray(terrain.vao);
	
	mat4 mvp =
		 perspective(PI / 2, 1.0f, 10.0f) *
		 scale(1.0f, aspect_ratio, 1.0f) *
		 rotate_x(player.vrot) *
		 rotate_y(-player.hrot) *
		 translate(-player.pos.x, -player.pos.y, -player.pos.z);

	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp.data);

	glDrawArrays(GL_TRIANGLES, 0, terrain.index_cnt);
}

void terrain_free_shader() {
	glDeleteProgram(prog);
}

void terrain_free(Terrain terrain) {
	glDeleteVertexArrays(1, &terrain.vao);
	glDeleteBuffers(1, &terrain.vbo);
}
