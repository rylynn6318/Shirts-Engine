#version 460 core
layout (location = 0) in vec3 inPosision;

out vec3 TexCoords;

uniform mat4 uProj;
uniform mat4 uView;

void main()
{
    TexCoords = inPosision;
    vec4 pos = uProj * uView * vec4(inPosision, 1.0);
    gl_Position = pos.xyww;
}  