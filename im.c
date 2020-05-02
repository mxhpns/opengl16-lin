#include "im.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void im_init() {
    stbi_set_flip_vertically_on_load(1);
}

GLuint im_load(const char *image_file_path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_file_path, &width, &height, &nrChannels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load image %s\n", image_file_path);
        exit(EXIT_FAILURE);
    }
    if (nrChannels < 3 || nrChannels > 4) {
        fprintf(stderr, "Number of channels should be 3 or 4 for %s\n", image_file_path);
        exit(EXIT_FAILURE);
    }
    glTexImage2D(GL_TEXTURE_2D,
                0, // level of detail
                GL_RGB, // result
                width,
                height,
                0, // should be always 0
                nrChannels == 3 ? GL_RGB : GL_RGBA, // source image
                GL_UNSIGNED_BYTE,
                data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
