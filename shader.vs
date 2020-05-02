#version 150

in vec4 pos;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

smooth out vec4 _color;
smooth out vec3 _pos;
smooth out vec3 _normal;
smooth out vec2 _texCoord;

void main() {
    _pos = vec3(model * pos);
    gl_Position = projection * view * vec4(_pos, 1.0f);
    _color = color;
    _normal = mat3(transpose(inverse(model))) * normal;
    
    _texCoord = texCoord;
}
