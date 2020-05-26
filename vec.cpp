#include "vec.h"

vec2 make_vec2(float x, float y) {
	vec2 r;
	r.x = x;
	r.y = y;
	return r;
}

vec3 make_vec3(float x, float y, float z) {
	vec3 r;
	r.x = x;
	r.y = y;
	r.z = z;
	return r;
}

vec4 make_vec4(float x, float y, float z, float w) {
	vec4 r;
	r.x = x;
	r.y = y;
	r.z = z;
	r.w = w;
	return r;
}

vec2 operator+(vec2 u, vec2 v) {
	return make_vec2(u.x + v.x, u.y + v.y);
}

vec3 operator+(vec3 u, vec3 v) {
	return make_vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec4 operator+(vec4 u, vec4 v) {
	return make_vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
}

vec2 operator-(vec2 u, vec2 v) {
	return make_vec2(u.x - v.x, u.y - v.y);
}

vec3 operator-(vec3 u, vec3 v) {
	return make_vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

vec4 operator-(vec4 u, vec4 v) {
	return make_vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
}

vec2 operator*(float x, vec2 v) {
	return make_vec2(x * v.x, x * v.y);
}

vec3 operator*(float x, vec3 v) {
	return make_vec3(x * v.x, x * v.y, x * v.z);
}

vec4 operator*(float x, vec4 v) {
	return make_vec4(x * v.x, x * v.y, x * v.z, x * v.w);
}

float dot(vec2 u, vec2 v) {
	return u.x * v.x + u.y * v.y;
}

float dot(vec3 u, vec3 v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float dot(vec4 u, vec4 v) {
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

vec3 cross(vec3 u, vec3 v) {
	return make_vec3(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x); }