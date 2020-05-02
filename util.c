#include "util.h"
#include <stdio.h>

GLuint prog;

char *readFile(const char *fileName) {
    FILE *f = fopen(fileName, "rb");
    if (!f) {
        perror("fopen failed");
        exit(1);
    }
    if (fseek(f, 0, SEEK_END) == -1) {
        perror("fseek failed");
        exit(1);
    }
    long len = ftell(f);
    if (len == -1) {
        perror("ftell failed");
        exit(1);
    }
    if (fseek(f, 0, SEEK_SET) == -1) {
        perror("fseek failed");
        exit(1);
    }
    char *res = malloc(len + 1);
    if (!res) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    if (fread(res, 1, len, f) != (size_t)len) {
        fprintf(stderr, "fread failed for file %s\n", fileName);
        exit(1);
    }
    if (fclose(f) == EOF) {
        perror("fclose failed");
        exit(1);
    }
    res[len] = 0;
    return res;
}

GLuint createShader(const char *shaderFile, GLenum shaderType) {
    const char *strShaderType;
    if (shaderType == GL_VERTEX_SHADER) {
        strShaderType = "vertex";
    } else if (shaderType == GL_GEOMETRY_SHADER) {
        strShaderType = "geometry";
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        strShaderType = "fragment";
    } else {
        fprintf(stderr, "Unrecognized shader type\n");
        exit(1);
    }
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        fprintf(stderr, "Error creating shader of type %s\n", strShaderType);
        exit(1);
    }
    GLchar *content = readFile(shaderFile);
    glShaderSource(shader, 1, (const GLchar **)&content, NULL);
    free(content);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        GLchar *info = malloc(sizeof(GLchar) * (infoLen + 1));
        glGetShaderInfoLog(shader, infoLen, NULL, info);
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, info);
        exit(1);
    }
    return shader;
}

void createProg(GLuint *shaders, int len) {
    int i = 0;
    prog = glCreateProgram();
    if (!prog) {
        fprintf(stderr, "Failed to create shader program\n");
        exit(1);
    }
    for (; i < len; i++) {
        glAttachShader(prog, shaders[i]);
    }
    glLinkProgram(prog);
    GLint status;
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLen;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLen);
        GLchar *info = malloc(sizeof(GLchar) * (infoLen + 1));
        glGetProgramInfoLog(prog, infoLen, NULL, info);
        fprintf(stderr, "Linker failure: %s\n", info);
        exit(1);
    }
}

