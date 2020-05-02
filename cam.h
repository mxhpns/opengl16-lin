#ifndef CAM_H
#define CAM_H

#include "m.h"

typedef struct {
    vec3 eye;
    vec3 look_dir;
    vec3 up;
    int forward_activated;
    int backward_activated;
    int left_activated;
    int right_activated;
    float yaw;
    float pitch;
    int last_x;
    int last_y;
    int first_mouse;
} Cam;

void cam_init(Cam *cam);

void cam_do_movement(Cam *cam, float delta_time);

void cam_motion(Cam *cam, int mx, int my, int the_w, int the_h, float sensitivity);

void cam_reshape(Cam *cam, int w, int h);

void cam_view(Cam *cam, mat4 view);

#endif