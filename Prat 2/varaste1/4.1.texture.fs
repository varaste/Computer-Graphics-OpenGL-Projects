#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float colorChangeValue;
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main(){
	

	FragColor = texture(texture1, TexCoord) *vec4(
												ourColor.x*vColor.x,
												ourColor.y*vColor.y,
												ourColor.z*vColor.z,
												1.0);
}
