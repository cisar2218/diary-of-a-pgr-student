#version 140

out vec4 color;

void main() {
	vec3 lul = normalize(gl_FragCoord.xyz);
	color = vec4(lul, 1.0f);
}