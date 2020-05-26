struct vec2 {
	float x;
	float y;
};

struct vec3 {
	float x;
	float y;
	float z;
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

vec2 make_vec2(float x, float y);
vec3 make_vec3(float x, float y, float z);
vec4 make_vec4(float x, float y, float z, float w);

vec2 operator+(vec2 u, vec2 v);
vec3 operator+(vec3 u, vec3 v);
vec4 operator+(vec4 u, vec4 v);

vec2 operator-(vec2 u, vec2 v);
vec3 operator-(vec3 u, vec3 v);
vec4 operator-(vec4 u, vec4 v);

vec2 operator*(float x, vec2 v);
vec3 operator*(float x, vec3 v);
vec4 operator*(float x, vec4 v);

float dot(vec2 u, vec2 v);
float dot(vec3 u, vec3 v);
float dot(vec4 u, vec4 v);

vec3 cross(vec3 u, vec3 v);
