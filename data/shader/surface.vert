#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

uniform mat4 modelViewPerspective;
uniform mat4 modelView;
uniform vec3 playerPos;

out float blend;

void main() {
	vec4 pos4 = modelViewPerspective * vec4(vPos, 1.0);
	gl_Position = pos4;
	vec3 pos = vec3(pos4);
	
	float constAtt = 16.0;
	float linearAtt = 0.01;
	float quadAtt = 0.001;
	float lightDist = length(playerPos - pos);
	float att = 1.0 / (constAtt + linearAtt * lightDist + quadAtt * lightDist * lightDist);
	vec3 normal = normalize(vNormal);
	blend = max(0.0, dot(normal, playerPos)) * att;
}