#version 330 core

in vec2 tex_pos;
out vec3 out_color;

uniform sampler2D tex;

float near(vec2 u, vec2 v) {
	return float(u.x == v.x || u.x == v.y || u.y == v.x);
}

void main() {
	vec2 dx = vec2(1.0 / 1920.0, 0.0);
	vec2 dy = vec2(0.0, 1.0 / 1080.0);

	vec2 h = texture(tex, tex_pos).xy;
	vec2 r = texture(tex, tex_pos + dx).xy;
	vec2 l = texture(tex, tex_pos - dx).xy;
	vec2 d = texture(tex, tex_pos + dy).xy;
	vec2 u = texture(tex, tex_pos - dy).xy;

	out_color = vec3(0.25 * (near(h, r) + near(h, l) + near(h, d) + near(h, u)));
	//out_color = vec3(h, 0.0);
	//out_color = vec3(1.0, 0.0, 0.0);
}
