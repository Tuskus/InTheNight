#version 330 core

uniform sampler2D tex;
uniform float threshold;
uniform float time;

in vec2 texCoord;

out vec4 fColor;

float rand(vec2 co) {
	float a = 12.9898;
	float b = 78.233;
	float c = 43758.5453;
	float dt = dot(co.xy, vec2(a, b));
	float sn = mod(dt, 3.14);
	return fract(sin(sn) * c);
}
vec3 rgb332(vec3 color) {
	color = pow(color, vec3(0.6));
	color = color * 8.0;
	color = floor(color);
	color = color / 8.0;
	color = pow(color, vec3(1.666));
	return color;
}
float hash(vec2 uv) {
	vec4 p = fract(vec4(uv.xyxy) * vec4(443.8975, 397.2973, 491.1871, 470.7827));
	p += dot(p.rgba, p + 19.19);
	return fract(vec4(p.r * p.g, p.g * p.a, p.b * p.r, p.g * p.r)).r;
}
float staticNoise(vec2 uv) {
	float y = uv.y;
	float s = time * 2.0;
	float v = rand(vec2(y * 0.01 + s, 1.0)) * rand(vec2(y * 0.011 + 1000.0 + s, 1.0)) * rand(vec2(y * 0.51 + 421 + s, 1.0));
	v *= hash(vec2(uv.x + time * 0.01, uv.y)) + 0.3;
	v += 0.3;
	if (v > 1.0) {
		v = 1.0;
	}
	if (v <= threshold) {
		v = 0.0;
	}
	return v;
}
void main() {
	float mag = 0.0009;
	
	vec2 rOffset = texCoord;
	rOffset.x = texCoord.x + rand(vec2(time / 1000.0 * 0.03, texCoord.y * 0.42)) * 0.001;
	rOffset.x += sin(rand(vec2(time / 1000.0 * 0.2, texCoord.y))) * mag;
	
	vec2 gOffset = texCoord;
	gOffset.x = texCoord.x + rand(vec2(time / 1000.0 * 0.004, texCoord.y * 0.002)) * 0.004;
	gOffset.x += sin(time / 1000.0 * 9.0) * mag;
	
	float r = texture2D(tex, rOffset).r;
	float g = texture2D(tex, gOffset).g;
	float b = texture2D(tex, texCoord).b;
	vec3 newColor = rgb332(vec3(r, g, b));

	newColor = clamp(newColor * 0.6 + 0.4 * newColor * newColor * 1.0, 0.0, 1.0);

	float vig = (0.0 + 1.0 * 16.0 * texCoord.x * texCoord.y * (1.0 - texCoord.x) * (1.0 - texCoord.y));
	newColor *= vec3(pow(vig, 0.3));

	newColor *= 1.0 + 0.01 * sin(110.0 * time / 1000.0);
	newColor *= 1.0 - 0.65 * vec3(clamp((mod(texCoord.x, 2.0) - 1.0) * 2.0, 0.0, 1.0));

	float col = 0.0;
	if (threshold < 1.0) {
		float numLines = 240.0;
		float lineY = 2.0 / numLines;
		vec2 uv = floor(texCoord / lineY) * lineY;
		col = staticNoise(uv);
	}
	
	fColor = vec4(newColor * (1.0 - col), 1.0);
}