#include "m.h"
#include <math.h>
#include <string.h>

// vec3

void m_vec3_scale(vec3 v, float s, vec3 dest) {
    dest[0] = v[0] * s;
    dest[1] = v[1] * s;
    dest[2] = v[2] * s;
}

float m_vec3_dot(vec3 a, vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float m_vec3_norm2(vec3 v) {
    return m_vec3_dot(v, v);
}

float m_vec3_norm(vec3 v) {
    return sqrtf(m_vec3_norm2(v));
}

void m_vec3_normalize(vec3 v) {
    float norm = m_vec3_norm(v);
    if (norm == 0.0f) {
        v[0] = v[1] = v[2] = 0.0f;
        return;
    }
    m_vec3_scale(v, 1.0f / norm, v);
}

void m_vec3_normalize_to(vec3 v, vec3 dest) {
    float norm = m_vec3_norm(v);
    if (norm == 0.0f) {
        dest[0] = dest[1] = dest[2] = 0.0f;
        return;
    }
    m_vec3_scale(v, 1.0f / norm, dest);
}


void m_vec3_cross(vec3 a, vec3 b, vec3 dest) {
    dest[0] = a[1] * b[2] - a[2] * b[1];
    dest[1] = a[2] * b[0] - a[0] * b[2];
    dest[2] = a[0] * b[1] - a[1] * b[0];
}

void m_vec3_crossn(vec3 a, vec3 b, vec3 dest) {
    m_vec3_cross(a, b, dest);
    m_vec3_normalize(dest);
}

void m_vec3_sub(vec3 a, vec3 b, vec3 dest) {
    dest[0] = a[0] - b[0];
    dest[1] = a[1] - b[1];
    dest[2] = a[2] - b[2];
}

void m_vec3_add(vec3 a, vec3 b, vec3 dest) {
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
}

// vec4


// mat4

void m_mat4_zero(mat4 mat) {
    memset(mat, 0, sizeof(mat4));
}

void m_perspective(float fovy, float aspect,
    float nearVal, float farVal,
    mat4  dest) {

    float f, fn;
    m_mat4_zero(dest);
    f = 1.0f / tanf(fovy * 0.5f);
    fn = 1.0f / (nearVal - farVal);
    dest[0][0] = f / aspect;
    dest[1][1] = f;
    dest[2][2] = (nearVal + farVal) * fn;
    dest[2][3] = -1.0f;
    dest[3][2] = 2.0f * nearVal * farVal * fn;
}

// https://habr.com/ru/post/327604/

void m_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest) {
    vec3 f, u, s;
    m_vec3_sub(center, eye, f);
    m_vec3_normalize(f);
    m_vec3_crossn(f, up, s);
    m_vec3_cross(s, f, u);
    dest[0][0] = s[0];
    dest[0][1] = u[0];
    dest[0][2] = -f[0];
    dest[1][0] = s[1];
    dest[1][1] = u[1];
    dest[1][2] = -f[1];
    dest[2][0] = s[2];
    dest[2][1] = u[2];
    dest[2][2] = -f[2];
    dest[3][0] = -m_vec3_dot(s, eye);
    dest[3][1] = -m_vec3_dot(u, eye);
    dest[3][2] = m_vec3_dot(f, eye);
    dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

void m_rotate_y_matr(float angle, mat4 dest) {
    mat4 t = MAT4_IDENTITY;
    float c, s;
    c = cosf(angle);
    s = sinf(angle);
    t[0][0] = c;
    t[0][2] = -s;
    t[2][0] = s;
    t[2][2] = c;
    memcpy(dest, t, sizeof(mat4));
}

void m_translate_matr(float x, float y, float z, mat4 dest) {
    mat4 t = MAT4_IDENTITY;
    t[3][0] = x;
    t[3][1] = y;
    t[3][2] = z;
    memcpy(dest, t, sizeof(mat4));
}

void m_mat4_mul(mat4 m1, mat4 m2, mat4 dest) {
    float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
        a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
        a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
        a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],

        b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2], b03 = m2[0][3],
        b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2], b13 = m2[1][3],
        b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2], b23 = m2[2][3],
        b30 = m2[3][0], b31 = m2[3][1], b32 = m2[3][2], b33 = m2[3][3];

    dest[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
    dest[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
    dest[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
    dest[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
    dest[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
    dest[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
    dest[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
    dest[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
    dest[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
    dest[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
    dest[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
    dest[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
    dest[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    dest[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    dest[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    dest[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}
