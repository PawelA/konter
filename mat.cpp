#include <string.h>

#include "vec.h"
#include "mat.h"

mat2 make_mat2(
	float a11, float a12,
	float a21, float a22) {

	mat2 r;

	r.data[0] = a11;
	r.data[1] = a21;
	r.data[2] = a12;
	r.data[3] = a22;

	return r;
}

mat3 make_mat3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33) {

	mat3 r;

	r.data[0] = a11;
	r.data[1] = a21;
	r.data[2] = a31;
	r.data[3] = a12;
	r.data[4] = a22;
	r.data[5] = a32;
	r.data[6] = a13;
	r.data[7] = a23;
	r.data[8] = a33;

	return r;
}

mat4 make_mat4(
	float a11, float a12, float a13, float a14,
	float a21, float a22, float a23, float a24,
	float a31, float a32, float a33, float a34,
	float a41, float a42, float a43, float a44) {

	mat4 r;

	r.data[0] = a11;
	r.data[1] = a21;
	r.data[2] = a31;
	r.data[3] = a41;
	r.data[4] = a12;
	r.data[5] = a22;
	r.data[6] = a32;
	r.data[7] = a42;
	r.data[8] = a13;
	r.data[9] = a23;
	r.data[10] = a33;
	r.data[11] = a43;
	r.data[12] = a14;
	r.data[13] = a24;
	r.data[14] = a34;
	r.data[15] = a44;

	return r;
}

mat2 mat2_zero() {
	mat2 r;
	memset(r.data, 0, 4 * sizeof(float));
	return r;
}

mat3 mat3_zero() {
	mat3 r;
	memset(r.data, 0, 9 * sizeof(float));
	return r;
}

mat4 mat4_zero() {
	mat4 r;
	memset(r.data, 0, 16 * sizeof(float));
	return r;
}

mat2 mat2_id() {
	mat2 r = mat2_zero();
	r.data[0] = 1.0f;
	r.data[3] = 1.0f;
	return r;
}

mat3 mat3_id() {
	mat3 r = mat3_zero();

	r.data[0] = 1.0f;
	r.data[4] = 1.0f;
	r.data[8] = 1.0f;

	return r;
}

mat4 mat4_id() {
	mat4 r = mat4_zero();

	r.data[0] = 1.0f;
	r.data[5] = 1.0f;
	r.data[10] = 1.0f;
	r.data[15] = 1.0f;

	return r;
}

mat2 operator+(mat2 m, mat2 n) {
	mat2 r;

	r.data[0] = m.data[0] + n.data[0];
	r.data[1] = m.data[1] + n.data[1];
	r.data[2] = m.data[2] + n.data[2];
	r.data[3] = m.data[3] + n.data[3];

	return r;
}

mat3 operator+(mat3 m, mat3 n) {
	mat3 r;
	
	r.data[0] = m.data[0] + n.data[0];
	r.data[1] = m.data[1] + n.data[1];
	r.data[2] = m.data[2] + n.data[2];
	r.data[3] = m.data[3] + n.data[3];
	r.data[4] = m.data[4] + n.data[4];
	r.data[5] = m.data[5] + n.data[5];
	r.data[6] = m.data[6] + n.data[6];
	r.data[7] = m.data[7] + n.data[7];
	r.data[8] = m.data[8] + n.data[8];

	return r;
}

mat4 operator+(mat4 m, mat4 n) {
	mat4 r;

	r.data[0] = m.data[0] + n.data[0];
	r.data[1] = m.data[1] + n.data[1];
	r.data[2] = m.data[2] + n.data[2];
	r.data[3] = m.data[3] + n.data[3];
	r.data[4] = m.data[4] + n.data[4];
	r.data[5] = m.data[5] + n.data[5];
	r.data[6] = m.data[6] + n.data[6];
	r.data[7] = m.data[7] + n.data[7];
	r.data[8] = m.data[8] + n.data[8];
	r.data[9] = m.data[9] + n.data[9];
	r.data[10] = m.data[10] + n.data[10];
	r.data[11] = m.data[11] + n.data[11];
	r.data[12] = m.data[12] + n.data[12];
	r.data[13] = m.data[13] + n.data[13];
	r.data[14] = m.data[14] + n.data[14];
	r.data[15] = m.data[15] + n.data[15];

	return r;
}

mat2 operator-(mat2 m, mat2 n) {
	mat2 r;

	r.data[0] = m.data[0] - n.data[0];
	r.data[1] = m.data[1] - n.data[1];
	r.data[2] = m.data[2] - n.data[2];
	r.data[3] = m.data[3] - n.data[3];

	return r;
}

mat3 operator-(mat3 m, mat3 n) {
	mat3 r;
	
	r.data[0] = m.data[0] - n.data[0];
	r.data[1] = m.data[1] - n.data[1];
	r.data[2] = m.data[2] - n.data[2];
	r.data[3] = m.data[3] - n.data[3];
	r.data[4] = m.data[4] - n.data[4];
	r.data[5] = m.data[5] - n.data[5];
	r.data[6] = m.data[6] - n.data[6];
	r.data[7] = m.data[7] - n.data[7];
	r.data[8] = m.data[8] - n.data[8];

	return r;
}

mat4 operator-(mat4 m, mat4 n) {
	mat4 r;

	r.data[0] = m.data[0] - n.data[0];
	r.data[1] = m.data[1] - n.data[1];
	r.data[2] = m.data[2] - n.data[2];
	r.data[3] = m.data[3] - n.data[3];
	r.data[4] = m.data[4] - n.data[4];
	r.data[5] = m.data[5] - n.data[5];
	r.data[6] = m.data[6] - n.data[6];
	r.data[7] = m.data[7] - n.data[7];
	r.data[8] = m.data[8] - n.data[8];
	r.data[9] = m.data[9] - n.data[9];
	r.data[10] = m.data[10] - n.data[10];
	r.data[11] = m.data[11] - n.data[11];
	r.data[12] = m.data[12] - n.data[12];
	r.data[13] = m.data[13] - n.data[13];
	r.data[14] = m.data[14] - n.data[14];
	r.data[15] = m.data[15] - n.data[15];

	return r;
}

mat2 operator*(mat2 m, mat2 n) {
	mat2 r = mat2_zero();

	r.data[0] += m.data[0] * n.data[0];
	r.data[0] += m.data[2] * n.data[1];
	
	r.data[1] += m.data[1] * n.data[0];
	r.data[1] += m.data[3] * n.data[1];
	
	r.data[2] += m.data[0] * n.data[2];
	r.data[2] += m.data[2] * n.data[3];
	
	r.data[3] += m.data[1] * n.data[2];
	r.data[3] += m.data[3] * n.data[3];

	return r;
}

mat3 operator*(mat3 m, mat3 n) {
	mat3 r = mat3_zero();

	r.data[0] += m.data[0] * n.data[0];
	r.data[0] += m.data[3] * n.data[1];
	r.data[0] += m.data[6] * n.data[2];
	
	r.data[1] += m.data[1] * n.data[0];
	r.data[1] += m.data[4] * n.data[1];
	r.data[1] += m.data[7] * n.data[2];
	
	r.data[2] += m.data[2] * n.data[0];
	r.data[2] += m.data[5] * n.data[1];
	r.data[2] += m.data[8] * n.data[2];
	
	r.data[3] += m.data[0] * n.data[3];
	r.data[3] += m.data[3] * n.data[4];
	r.data[3] += m.data[6] * n.data[5];
	
	r.data[4] += m.data[1] * n.data[3];
	r.data[4] += m.data[4] * n.data[4];
	r.data[4] += m.data[7] * n.data[5];
	
	r.data[5] += m.data[2] * n.data[3];
	r.data[5] += m.data[5] * n.data[4];
	r.data[5] += m.data[8] * n.data[5];
	
	r.data[6] += m.data[0] * n.data[6];
	r.data[6] += m.data[3] * n.data[7];
	r.data[6] += m.data[6] * n.data[8];
	
	r.data[7] += m.data[1] * n.data[6];
	r.data[7] += m.data[4] * n.data[7];
	r.data[7] += m.data[7] * n.data[8];
	
	r.data[8] += m.data[2] * n.data[6];
	r.data[8] += m.data[5] * n.data[7];
	r.data[8] += m.data[8] * n.data[8];

	return r;
}

mat4 operator*(mat4 m, mat4 n) {
	mat4 r = mat4_zero();

	r.data[0] += m.data[0]  * n.data[0];
	r.data[0] += m.data[4]  * n.data[1];
	r.data[0] += m.data[8]  * n.data[2];
	r.data[0] += m.data[12] * n.data[3];
	
	r.data[1] += m.data[1]  * n.data[0];
	r.data[1] += m.data[5]  * n.data[1];
	r.data[1] += m.data[9]  * n.data[2];
	r.data[1] += m.data[13] * n.data[3];
	
	r.data[2] += m.data[2]  * n.data[0];
	r.data[2] += m.data[6]  * n.data[1];
	r.data[2] += m.data[10] * n.data[2];
	r.data[2] += m.data[14] * n.data[3];
	
	r.data[3] += m.data[3]  * n.data[0];
	r.data[3] += m.data[7]  * n.data[1];
	r.data[3] += m.data[11] * n.data[2];
	r.data[3] += m.data[15] * n.data[3];
	
	r.data[4] += m.data[0]  * n.data[4];
	r.data[4] += m.data[4]  * n.data[5];
	r.data[4] += m.data[8]  * n.data[6];
	r.data[4] += m.data[12] * n.data[7];
	
	r.data[5] += m.data[1]  * n.data[4];
	r.data[5] += m.data[5]  * n.data[5];
	r.data[5] += m.data[9]  * n.data[6];
	r.data[5] += m.data[13] * n.data[7];
	
	r.data[6] += m.data[2]  * n.data[4];
	r.data[6] += m.data[6]  * n.data[5];
	r.data[6] += m.data[10] * n.data[6];
	r.data[6] += m.data[14] * n.data[7];
	
	r.data[7] += m.data[3]  * n.data[4];
	r.data[7] += m.data[7]  * n.data[5];
	r.data[7] += m.data[11] * n.data[6];
	r.data[7] += m.data[15] * n.data[7];

	r.data[8] += m.data[0]  * n.data[8];
	r.data[8] += m.data[4]  * n.data[9];
	r.data[8] += m.data[8]  * n.data[10];
	r.data[8] += m.data[12] * n.data[11];
	
	r.data[9] += m.data[1]  * n.data[8];
	r.data[9] += m.data[5]  * n.data[9];
	r.data[9] += m.data[9]  * n.data[10];
	r.data[9] += m.data[13] * n.data[11];
	
	r.data[10] += m.data[2]  * n.data[8];
	r.data[10] += m.data[6]  * n.data[9];
	r.data[10] += m.data[10] * n.data[10];
	r.data[10] += m.data[14] * n.data[11];
	
	r.data[11] += m.data[3]  * n.data[8];
	r.data[11] += m.data[7]  * n.data[9];
	r.data[11] += m.data[11] * n.data[10];
	r.data[11] += m.data[15] * n.data[11];
	
	r.data[12] += m.data[0]  * n.data[12];
	r.data[12] += m.data[4]  * n.data[13];
	r.data[12] += m.data[8]  * n.data[14];
	r.data[12] += m.data[12] * n.data[15];
	
	r.data[13] += m.data[1]  * n.data[12];
	r.data[13] += m.data[5]  * n.data[13];
	r.data[13] += m.data[9]  * n.data[14];
	r.data[13] += m.data[13] * n.data[15];
	
	r.data[14] += m.data[2]  * n.data[12];
	r.data[14] += m.data[6]  * n.data[13];
	r.data[14] += m.data[10] * n.data[14];
	r.data[14] += m.data[14] * n.data[15];
	
	r.data[15] += m.data[3]  * n.data[12];
	r.data[15] += m.data[7]  * n.data[13];
	r.data[15] += m.data[11] * n.data[14];
	r.data[15] += m.data[15] * n.data[15];

	return r;
}

mat2 operator*(float x, mat2 m) {
	mat2 r;

	r.data[0] = x * m.data[0];
	r.data[1] = x * m.data[1];
	r.data[2] = x * m.data[2];
	r.data[3] = x * m.data[3];

	return r;
}

mat3 operator*(float x, mat3 m) {
	mat3 r;

	r.data[0] = x * m.data[0];
	r.data[1] = x * m.data[1];
	r.data[2] = x * m.data[2];
	r.data[3] = x * m.data[3];
	r.data[4] = x * m.data[4];
	r.data[5] = x * m.data[5];
	r.data[6] = x * m.data[6];
	r.data[7] = x * m.data[7];
	r.data[8] = x * m.data[8];

	return r;
}

mat4 operator*(float x, mat4 m) {
	mat4 r;
	
	r.data[0] = x * m.data[0];
	r.data[1] = x * m.data[1];
	r.data[2] = x * m.data[2];
	r.data[3] = x * m.data[3];
	r.data[4] = x * m.data[4];
	r.data[5] = x * m.data[5];
	r.data[6] = x * m.data[6];
	r.data[7] = x * m.data[7];
	r.data[8] = x * m.data[8];
	r.data[9] = x * m.data[9];
	r.data[10] = x * m.data[10];
	r.data[11] = x * m.data[11];
	r.data[12] = x * m.data[12];
	r.data[13] = x * m.data[13];
	r.data[14] = x * m.data[14];
	r.data[15] = x * m.data[15];

	return r;
}

vec2 operator*(mat2 m, vec2 v) {
	vec2 r;

	r.x = m.data[0] * v.x + m.data[2] * v.y;
	r.y = m.data[1] * v.x + m.data[3] * v.y;

	return r;
}

vec3 operator*(mat3 m, vec3 v) {
	vec3 r;

	r.x = m.data[0] * v.x + m.data[3] * v.y + m.data[6] * v.z;
	r.y = m.data[1] * v.x + m.data[4] * v.y + m.data[7] * v.z;
	r.z = m.data[2] * v.x + m.data[5] * v.y + m.data[8] * v.z;

	return r;
}

vec4 operator*(mat4 m, vec4 v) {
	vec4 r;

	r.x = m.data[0] * v.x + m.data[4] * v.y + m.data[8]  * v.z + m.data[12] * v.w;
	r.y = m.data[1] * v.x + m.data[5] * v.y + m.data[9]  * v.z + m.data[13] * v.w;
	r.z = m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w;
	r.w = m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w;

	return r;
}
