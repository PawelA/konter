#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "gl.h"
#include "window.h"

#include "vec.h"
#include "mesh.h"
#include "player.h"
#include "terrain.h"
#include "target.h"

static float player_up;
static float player_fwd;
static float player_left;

static void keypress(int code, int state) {
	float delta = state ? 1.0f : -1.0f;
	switch (code) {
	case 'w': player_fwd  += delta; break;
	case 'a': player_left += delta; break;
	case 's': player_fwd  -= delta; break;
	case 'd': player_left -= delta; break;
	case ' ': player_up   += delta; break;
	case SHIFT: player_up -= delta; break;
	}
}

int main() {
	Mesh mesh;
	//Mesh mesh_exp;
	Terrain terrain;
	Player player;
	Target target;
	int width = 1920;
	int height = 1080;

	window_init();
	if (!window_create(width, height, WINDOW_FULLSCREEN))
		exit(1);
	window_set_cursor(0);
	window_set_warp(1);
	window_register_key_cb(keypress);

	loadgl();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, width, height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	if (!mesh_load(&mesh, "models/bunny.obj")) exit(1);
	//mesh_perturb(&mesh, 0.001f);
	//mesh_expand(mesh, &mesh_exp, 1.0f, 1.0f, 1.0f);

	if (!terrain_init_shader()) exit(1);
	if (!target_init_shader()) exit(1);

	terrain_init(&terrain, mesh);
	target_init(&target, width, height);

	player = make_player(make_vec3(0.0f, 10.0f, 0.0f));

	while (window_alive()) {
		int x, y;
		window_get_cursor_delta(&x, &y);
		player_rotate(&player, x, y);
		player_move(&player, player_fwd, player_left, player_up, 1.0f / 60.0f);
		//player_apply_collision(&player, mesh_exp);

		glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain_render(terrain, player, (float) width / height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		target_render(target);

		window_swap_buffers();
		window_poll_events();
	}

	terrain_free_shader();
	terrain_free(terrain);

	mesh_free(mesh);

	return 0;
}
