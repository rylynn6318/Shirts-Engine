#version 460 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;

void main()
{
	gl_Position = uProj* uView * uModel * vec4(inPosition, 1.0);
	TexCoord = vec2(inTexCoord.x, inTexCoord.y);
}