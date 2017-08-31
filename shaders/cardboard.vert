#version 120

attribute vec2 position;
attribute vec3 texCoord;

varying vec2 vTexCoord;

uniform vec4 viewportOffsetScale[2];

void main() {
	vec4 viewport = viewportOffsetScale[int(texCoord.z)];
	vTexCoord = (texCoord.xy * viewport.zw) + viewport.xy;
	vTexCoord.y = 1 - vTexCoord.y;
	gl_Position = vec4( position, 1.0, 1.0 );
}