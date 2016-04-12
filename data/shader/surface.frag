#version 330 core

uniform vec3 light;
uniform vec3 dark;

in float blend;

out vec4 fColor;

vec3 yiqToRGB(vec3 yiq) {
	float r = yiq.r + 0.956 * yiq.g + 0.621 * yiq.b;
	float g = yiq.r - 0.272 * yiq.g - 0.647 * yiq.b;
	float b = yiq.r - 1.106 * yiq.g + 1.703 * yiq.b;
	return vec3(r, g, b);
}
void main() {
	float dist = gl_FragCoord.z / gl_FragCoord.w;
	float alpha = 1.0;
	if (dist > 24.0) {
		alpha = 4.0f - dist / 8.0f;
	}
	fColor = vec4(yiqToRGB(mix(dark, light, blend)) * (1.0 - dist / 32.0), alpha);
}