#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
   vec4 viewPos = view * model * vec4(position, 1.0);
   gl_Position = projection * viewPos;

   v_TexCoords = uv;
   v_Normal = normalMatrix * normal;
   v_FragPos = viewPos.xyz;
}
