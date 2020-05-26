struct Player {
	vec3 pos;
	vec3 vel;
	float hrot;
	float vrot;
	bool on_ground;
};

Player make_player(vec3 pos);

void player_rotate(Player *player, int dx, int dy);

void player_move(Player *player, float fwd, float left, float up, float dt);

void player_apply_collision(Player *player, Mesh mesh);
