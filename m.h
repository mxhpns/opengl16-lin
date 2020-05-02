#ifndef M_H
#define M_H

#define TO_RAD(deg) (((deg) * 3.141593f / 180.0f))

// vec3

typedef float vec3[3];

void m_vec3_scale(vec3 v, float s, vec3 dest);

float m_vec3_dot(vec3 a, vec3 b);

float m_vec3_norm2(vec3 v);

float m_vec3_norm(vec3 v);

void m_vec3_normalize(vec3 v);

void m_vec3_normalize_to(vec3 v, vec3 dest);

void m_vec3_cross(vec3 a, vec3 b, vec3 dest);

void m_vec3_crossn(vec3 a, vec3 b, vec3 dest);

void m_vec3_sub(vec3 a, vec3 b, vec3 dest);

void m_vec3_add(vec3 a, vec3 b, vec3 dest);

// vec4

typedef float vec4[4];

// mat4

#define MAT4_IDENTITY   {{1.0f, 0.0f, 0.0f, 0.0f},\
                         {0.0f, 1.0f, 0.0f, 0.0f},\
                         {0.0f, 0.0f, 1.0f, 0.0f},\
                         {0.0f, 0.0f, 0.0f, 1.0f}}

typedef vec4 mat4[4];

void m_perspective(float fovy, float aspect, float nearVal, float farVal,
    mat4  dest);

void m_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest);

void m_rotate_y_matr(float angle, mat4 dest);

void m_translate_matr(float x, float y, float z, mat4 dest);

void m_mat4_mul(mat4 m1, mat4 m2, mat4 dest);

#endif
