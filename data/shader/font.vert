#version 330 core

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec2 vTexCoord;

uniform vec2 offset;
uniform vec2 texOffset;

out vec2 texCoord;

void main() {
	vec2 pos = vPos;
	texCoord = vTexCoord + texOffset;
	gl_Position = vec4(pos + offset, 0.0, 1.0);
}