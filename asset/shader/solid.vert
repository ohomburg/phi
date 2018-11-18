#version 430
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 padding;

layout(location = 0) uniform mat4 mModel;
layout(location = 1) uniform mat4 mViewProject;

void main() {
    vec4 vM = mModel * vec4(vPosition, 1.0);
    gl_Position = mViewProject * vM;
}
