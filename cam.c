#include "cam.h"
#include "m.h"
#include "string.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>

void cam_init(Cam *cam) {
    memset(cam, 0, sizeof(Cam));
    cam->eye[2] = 10.0f;
    cam->look_dir[2] = -1.0f;
    cam->up[1] = 1.0f;
    cam->yaw = -90.0f;
    cam->first_mouse = 1;
}

void cam_do_movement(Cam *cam, float delta_time) {
    float camSpeed = 0.01f * delta_time;
    if (cam->forward_activated) {
        vec3 delta;
        memcpy(delta, cam->look_dir, sizeof(vec3));
        //delta[1] = 0.f;
        m_vec3_normalize_to(delta, delta);
        m_vec3_scale(delta, camSpeed, delta);
        m_vec3_add(cam->eye, delta, cam->eye);
    }
    if (cam->backward_activated) {
        vec3 delta;
        memcpy(delta, cam->look_dir, sizeof(vec3));
        //delta[1] = 0.f;
        m_vec3_normalize_to(delta, delta);
        m_vec3_scale(delta, camSpeed, delta);
        m_vec3_sub(cam->eye, delta, cam->eye);
    }
    if (cam->left_activated) {
        vec3 right;
        m_vec3_crossn(cam->look_dir, cam->up, right);
        m_vec3_normalize(right);
        m_vec3_scale(right, camSpeed, right);
        m_vec3_sub(cam->eye, right, cam->eye);
    }
    if (cam->right_activated) {
        vec3 right;
        m_vec3_crossn(cam->look_dir, cam->up, right);
        m_vec3_normalize(right);
        m_vec3_scale(right, camSpeed, right);
        m_vec3_add(cam->eye, right, cam->eye);
    }
}

#include <stdio.h>
void cam_motion(Cam *cam, int mx, int my, int the_w, int the_h, float sensitivity) {
    //printf("%d %d\n", mx, my);
    static int warped = 0;
    if (warped) {
        //printf("miss\n");
        if (mx != cam->last_x || my != cam->last_y) {
            return;
        }
        warped = 0;
    }
    if (the_w == 0) {
        return;
    }
    if (cam->first_mouse) {
        cam->first_mouse = 0;
        cam->last_x = mx;
        cam->last_y = my;
    }
    float offset_x = (float)(mx - cam->last_x);
    float offset_y = (float)(cam->last_y - my);
    cam->last_x = mx;
    cam->last_y = my;

    offset_x *= sensitivity;
    offset_y *= sensitivity;

    cam->yaw += offset_x;
    cam->pitch += offset_y;

    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    }
    if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }

    cam->look_dir[0] = cosf(TO_RAD(cam->yaw)) * cosf(TO_RAD(cam->pitch));
    cam->look_dir[1] = sinf(TO_RAD(cam->pitch));
    cam->look_dir[2] = sinf(TO_RAD(cam->yaw)) * cosf(TO_RAD(cam->pitch));
    m_vec3_normalize(cam->look_dir);

    int margin = 100;
    if (mx < margin || mx > the_w - margin ||
        my < margin || my > the_h - margin) {
        cam->last_x = the_w / 2;
        cam->last_y = the_h / 2;
        warped = 1;
        glutWarpPointer(cam->last_x, cam->last_y);
        //printf("changed: %d %d %d %d\n", mx, my, cam->last_x, cam->last_y);
    }
}

void cam_reshape(Cam *cam, int w, int h) {
    cam->last_x = w / 2;
    cam->last_y = h / 2;
    glutWarpPointer(cam->last_x, cam->last_y);
}

void cam_view(Cam *cam, mat4 view) {
    vec3 center;
    m_vec3_add(cam->eye, cam->look_dir, center);
    m_lookat(cam->eye, center, cam->up, view);
}
