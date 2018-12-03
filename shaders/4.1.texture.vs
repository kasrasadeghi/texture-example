#version 330 core
// non instanced
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoord;

// instanced
layout (location = 3) in vec2 posOffset;
layout (location = 4) in float texOffset;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 projection;

// mat2 rotate(in float theta) {
// 	mat2 result {
// 		cos(theta), sin(theta),
// 	 -sin(theta), cos(theta)
// 	};
// 	return result;
// }

void main()
{
	gl_Position = projection * vec4(aPos + posOffset, 0.0, 1.0);
	ourColor = aColor;
	TexCoord = texCoord + vec2(texOffset, 0);
}