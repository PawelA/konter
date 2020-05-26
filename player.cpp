#include <math.h>

#include "vec.h"
#include "mesh.h"
#include "player.h"
#include "constants.h"

Player make_player(vec3 pos) {
	Player player;

	player.pos = pos;
	player.vel = make_vec3(0.0f, 0.0f, 0.0f);
	player.hrot = 0.0f;
	player.vrot = 0.0f;
	player.on_ground = false;

	return player;
}

void player_rotate(Player *player, int dx, int dy) {
	const float mouse_scale = 0.001f;

	player->hrot += dx * mouse_scale;
	player->vrot += dy * mouse_scale;

	if (player->vrot < -PI/2) player->vrot = -PI/2;
	if (player->vrot >  PI/2) player->vrot =  PI/2;
}

void player_move(Player *player, float fwd, float left, float up, float dt) {
	const float walk_speed = 5.0f;
	const float s = walk_speed * sinf(player->hrot);
	const float c = walk_speed * cosf(player->hrot);

	player->vel.x = fwd * s - left * c;
	player->vel.y = up * walk_speed;
	player->vel.z = -fwd * c - left * s;

	player->pos = player->pos + dt * player->vel;
}

void player_apply_collision(Player *player, Mesh mesh) {
	vec3 p = player->pos;

	for (int i = 0; i < mesh.index_cnt; i += 3) {
		vec3 t1 = mesh.verts[mesh.indices[i]];
		vec3 t2 = mesh.verts[mesh.indices[i+1]];
		vec3 t3 = mesh.verts[mesh.indices[i+2]];
		vec3 n = cross(t2-t1, t3-t1);

		float n_rp = dot(n, p - t1);
		vec3 proj = n_rp / dot(n, n) * n;

		if (n_rp >= 0.0f || dot(proj, proj) >= 0.1f)
			continue;

		vec3 pp = p - proj;
		t1 = t1 - pp;
		t2 = t2 - pp;
		t3 = t3 - pp;

		if (dot(n, cross(t1, t2)) < 0.0f) continue;
		if (dot(n, cross(t2, t3)) < 0.0f) continue;
		if (dot(n, cross(t3, t1)) < 0.0f) continue;

		p = pp;
	}

	player->pos = p;
}
