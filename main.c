#include <GL/glew.h>
#include <GL/freeglut.h>
#include "util.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "m.h"
#include "cam.h"
#include "im.h"

GLuint vao;
GLuint vbo;
GLuint ibo;
GLuint projectionLoc, viewLoc, viewPosLoc, lightPosLoc, lightColorLoc;
GLuint modelLoc, texture1Loc;
GLuint posAttr, colorAttr, normalAttr, texCoordAttr;

GLuint texture1;

Cam cam;
float last_frame;
float delta_time;
int the_w, the_h;

void setUniformLocations() {
    modelLoc = glGetUniformLocation(prog, "model");
    viewLoc = glGetUniformLocation(prog, "view");
    projectionLoc = glGetUniformLocation(prog, "projection");
    viewPosLoc = glGetUniformLocation(prog, "viewPos");
    lightPosLoc = glGetUniformLocation(prog, "lightPos");
    lightColorLoc = glGetUniformLocation(prog, "lightColor");
    texture1Loc = glGetUniformLocation(prog, "texture1Loc");

    posAttr = glGetAttribLocation(prog, "pos");
    colorAttr = glGetAttribLocation(prog, "color");
    normalAttr = glGetAttribLocation(prog, "normal");
    texCoordAttr = glGetAttribLocation(prog, "texCoord");
}

void createBuffer();
void initVao();

void init() {
    createBuffer();

    GLuint shaders[] = {
        createShader("shader.vs", GL_VERTEX_SHADER),
        createShader("shader.fs", GL_FRAGMENT_SHADER)
    };
    int len = sizeof(shaders) / sizeof(shaders[0]);
    createProg(shaders, len);
    int i = 0;
    for (; i < len; i++) {
        glDeleteShader(shaders[i]);
    }

    setUniformLocations();

    initVao();

    texture1 = im_load("textures/wall.jpg");
    glUseProgram(prog);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(texture1Loc, 0);
    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    // glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
}

void createBuffer() {
    GLushort indices[] = {
        // front plane
        0, 1, 2,
        0, 2, 3,
        // top plane
        4, 5, 6,
        4, 6, 7,
        // bottom plane
        8, 9, 10,
        8, 10, 11,
        // back plane
        12, 13, 14,
        12, 14, 15,
        // left plane
        16, 17, 18,
        16, 18, 19,
        // right plane
        20, 21, 22,
        20, 22, 23
    };
    float vertices[] = {
        // 1. vertices
        // front plane:
       -0.5f, -0.5f,  0.5f,// left-bottom-front
       -0.5f,  0.5f,  0.5f,// left-top-front
        0.5f,  0.5f,  0.5f,// right-top-front
        0.5f, -0.5f,  0.5f,// right-bottom-front

        // top plane:
       -0.5f,  0.5f,  0.5f,// left-top-front
       -0.5f,  0.5f, -0.5f,// left-top-back
        0.5f,  0.5f, -0.5f,// right-top-back
        0.5f,  0.5f,  0.5f,// right-top-front
       
        // bottom plane:
       -0.5f, -0.5f, -0.5f,// left-bottom-back
       -0.5f, -0.5f,  0.5f,// left-bottom-front
        0.5f, -0.5f,  0.5f,// right-bottom-front
        0.5f, -0.5f, -0.5f,// right-bottom-back

        // back plane:
        0.5f, -0.5f, -0.5f,// right-bottom-back
        0.5f,  0.5f, -0.5f,// right-top-back
       -0.5f,  0.5f, -0.5f,// left-top-back
       -0.5f, -0.5f, -0.5f,// left-bottom-back

        // left plane:
       -0.5f, -0.5f, -0.5f,// left-bottom-back
       -0.5f,  0.5f, -0.5f,// left-top-back
       -0.5f,  0.5f,  0.5f,// left-top-front
       -0.5f, -0.5f,  0.5f,// left-bottom-front

        // right plane:
        0.5f, -0.5f,  0.5f,// right-bottom-front
        0.5f,  0.5f,  0.5f,// right-top-front
        0.5f,  0.5f, -0.5f,// right-top-back
        0.5f, -0.5f, -0.5f,// right-bottom-back
                          
        // 2. colors
        // front plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red

        // top plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
       
        // bottom plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red

        // back plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red

        // left plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red

        // right plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        /*
        // front plane:
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red
        1.0f, 0.0f, 0.0f, 1.0f, // red

        // top plane:
        0.0f, 1.0f, 0.0f, 1.0f, // green
        0.0f, 1.0f, 0.0f, 1.0f, // green
        0.0f, 1.0f, 0.0f, 1.0f, // green
        0.0f, 1.0f, 0.0f, 1.0f, // green
       
        // bottom plane:
        1.0f, 1.0f, 0.0f, 1.0f, // yellow
        1.0f, 1.0f, 0.0f, 1.0f, // yellow
        1.0f, 1.0f, 0.0f, 1.0f, // yellow
        1.0f, 1.0f, 0.0f, 1.0f, // yellow

        // back plane:
        0.0f, 1.0f, 1.0f, 1.0f, // aqua
        0.0f, 1.0f, 1.0f, 1.0f, // aqua
        0.0f, 1.0f, 1.0f, 1.0f, // aqua
        0.0f, 1.0f, 1.0f, 1.0f, // aqua

        // left plane:
        0.5f, 0.0f, 0.0f, 1.0f, // dark-red
        0.5f, 0.0f, 0.0f, 1.0f, // dark-red
        0.5f, 0.0f, 0.0f, 1.0f, // dark-red
        0.5f, 0.0f, 0.0f, 1.0f, // dark-red

        // right plane:
        0.0f, 0.0f, 0.5f, 1.0f,  // dark-blue
        0.0f, 0.0f, 0.5f, 1.0f,  // dark-blue
        0.0f, 0.0f, 0.5f, 1.0f,  // dark-blue
        0.0f, 0.0f, 0.5f, 1.0f,  // dark-blue
        */

        // 3. normals
        // front plane
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // top plane
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // bottom plane
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // back plane
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // left plane
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // right plane
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // ! textures
        // front
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // top
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // bottom
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // back
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // left
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // right
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initVao() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex array object will store next things:
    // 1. Binding to GL_ELEMENT_ARRAY_BUFFER
    // 2. Enabled vertex attrib arrays
    // 3. Vertex attrib pointer settings

    glEnableVertexAttribArray(posAttr);
    glEnableVertexAttribArray(colorAttr);
    glEnableVertexAttribArray(normalAttr);
    glEnableVertexAttribArray(texCoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glVertexAttribPointer will use GL_ARRAY_BUFFER
    // , which was set by glBindBuffer
    glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(colorAttr, 4, GL_FLOAT, GL_FALSE, 0, (void*)(6*4*3*sizeof(float)));
    glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, 0, (void*)((6*4*3 + 6*4*4)*sizeof(float)));
    glVertexAttribPointer(texCoordAttr, 2, GL_FLOAT, GL_FALSE, 0, (void*)((6*4*3 + 6*4*4 + 6*4*3)*sizeof(float)));
    // Binding to GL_ARRAY_BUFFER will not be stored in
    // the vertex array object,
    // but it was already defined for glVertexAttribPointer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // This will be used by glDrawElements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);

}

void set_model(float elapsed, mat4 dest) {
    float part = fmodf(elapsed, 5000.0f) / 5000.0f;
    float angle = 3.14159f*2.0f * part;
    m_rotate_y_matr(angle, dest);
}

void display() {
    float elapsed = glutGet(GLUT_ELAPSED_TIME);
    delta_time = elapsed - last_frame;
    last_frame = elapsed;
    cam_do_movement(&cam, delta_time);
    glClearColor(0.16f, 0.03f, 0.34f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(prog);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);

    glUniform3fv(viewPosLoc, 1, cam.eye);
    glUniform3f(lightPosLoc, 5.0f, 5.0f, 5.0f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    mat4 view;
    cam_view(&cam, view);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);

    mat4 model = MAT4_IDENTITY;
    float scale = 5.0f;
    model[0][0] = scale;
    model[1][1] = scale;
    model[2][2] = scale;
    //set_model(elapsed, model);
    mat4 trans;
    m_translate_matr(0, 0, 0, trans);
    m_mat4_mul(trans, model, model);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat*)model);

    glDrawElements(GL_TRIANGLES, 6*2*3, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    cam_reshape(&cam, w, h);
    the_w = w;
    the_h = h;
    float fovy = TO_RAD(45);
    float aspect = w / (float)h;
    float nearVal = 0.2f;
    float farVal = 1000.0f;

    mat4 projection;
    m_perspective(fovy, aspect, nearVal, farVal, projection);
    glUseProgram(prog);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat*)projection);
    glUseProgram(0);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    if (key == 27) {
        glutLeaveMainLoop();
    } else if (key == 'w') {
        cam.forward_activated = 1;
    } else if (key == 's') {
        cam.backward_activated = 1;
    } else if (key == 'a') {
        cam.left_activated = 1;
    } else if (key == 'd') {
        cam.right_activated = 1;
    }
    glutPostRedisplay();
}

void keyboard_release(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    if (key == 27) {
        glutLeaveMainLoop();
    } else if (key == 'w') {
        cam.forward_activated = 0;
    } else if (key == 's') {
        cam.backward_activated = 0;
    } else if (key == 'a') {
        cam.left_activated = 0;
    } else if (key == 'd') {
        cam.right_activated = 0;
    }
}

void motion(int mx, int my) {
    cam_motion(&cam, mx, my, the_w, the_h, 0.30f);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }
    printf("GL version: %s\n", glGetString(GL_VERSION));

    im_init();
    cam_init(&cam);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    // glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_release);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    init();
    glutMainLoop();
    return 0;
}
