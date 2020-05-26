struct mat2 {
	float data[4];
};

struct mat3 {
	float data[9];
};

struct mat4 {
	float data[16];
};

mat2 make_mat2(
	float a11, float a12,
	float a21, float a22);

mat3 make_mat3(
	float a11, float a12, float a13,
	float a21, float a22, float a23,
	float a31, float a32, float a33);

mat4 make_mat4(
	float a11, float a12, float a13, float a14,
	float a21, float a22, float a23, float a24,
	float a31, float a32, float a33, float a34,
	float a41, float a42, float a43, float a44);

mat2 mat2_zero();
mat3 mat3_zero();
mat4 mat4_zero();

mat2 mat2_id();
mat3 mat3_id();
mat4 mat4_id();

mat2 operator+(mat2 m, mat2 n);
mat3 operator+(mat3 m, mat3 n);
mat4 operator+(mat4 m, mat4 n);

mat2 operator-(mat2 m, mat2 n);
mat3 operator-(mat3 m, mat3 n);
mat4 operator-(mat4 m, mat4 n);

mat2 operator*(float x, mat2 m);
mat3 operator*(float x, mat3 m);
mat4 operator*(float x, mat4 m);

mat2 operator*(mat2 m, mat2 n);
mat3 operator*(mat3 m, mat3 n);
mat4 operator*(mat4 m, mat4 n);

vec2 operator*(mat2 m, vec2 v);
vec3 operator*(mat3 m, vec3 v);
vec4 operator*(mat4 m, vec4 v);
