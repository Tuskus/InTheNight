#version 330 core

uniform sampler2D diffuseTex;

in vec2 texCoord;

out vec4 fColor;

void main() {
	float alpha = texture(diffuseTex, texCoord).a;
	if (alpha == 0.0) {
		discard;
	} else {
		fColor = vec4(texture(diffuseTex, texCoord).rgb, alpha);
	}
}