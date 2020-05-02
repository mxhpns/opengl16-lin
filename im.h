#ifndef IM_H
#define IM_H

#include <GL/glew.h>
#include <GL/freeglut.h>

void im_init();

GLuint im_load(const char *image_file_path);

#endif
