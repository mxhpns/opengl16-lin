#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <GL/freeglut.h>

extern GLuint prog;

char *readFile(const char *fileName);

GLuint createShader(const char *shaderFile, GLenum shaderType);

void createProg(GLuint *shaders, int len);

#endif
