#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

layout (location = 0) out vec3 FragmentColor;

layout (binding = 0) uniform UniformBufferObject {
    mat4 Projection;
    mat4 Model;
    mat4 View;
} UBO;

void main() {
    gl_Position = UBO.Projection * UBO.View * UBO.Model * vec4(VertexPosition, 1.0f);
    FragmentColor = VertexColor;
}