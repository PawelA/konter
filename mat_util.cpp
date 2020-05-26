#include <math.h>

#include "vec.h"
#include "mat.h"

mat4 translate(float x, float y, float z) {
	return make_mat4(
		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 scale(float x, float y, float z) {
	return make_mat4(
		x   , 0.0f, 0.0f, 0.0f,
		0.0f, y   , 0.0f, 0.0f,
		0.0f, 0.0f, z   , 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
		
}

mat4 rotate_x(float a) {
	float s = sinf(a);
	float c = cosf(a);

	return make_mat4(
		1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, c   , -s   , 0.0f,
		0.0f, s   ,  c   , 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f);
}

mat4 rotate_y(float a) {
	float s = sinf(a);
	float c = cosf(a);

	return make_mat4(
		c   , 0.0f, -s   , 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		s   , 0.0f,  c   , 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f);
}

mat4 rotate_z(float a) {
	float s = sinf(a);
	float c = cosf(a);

	return make_mat4(
		c   , -s   , 0.0f, 0.0f,
		s   ,  c   , 0.0f, 0.0f,
		0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f, 1.0f);
}

mat4 perspective(float fov, float near, float far) {
	float t = 1.0f / tanf(fov * 0.5f);
	float r = far / (near - far);

	return make_mat4(
		t   , 0.0f, 0.0f    ,  0.0f,
		0.0f, t   , 0.0f    ,  0.0f,
		0.0f, 0.0f, r       , -1.0f,
		0.0f, 0.0f, r * near,  0.0f);
}
