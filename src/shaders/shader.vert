#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec3 modelPos;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
   vec4 worldPos = vec4(modelPos, 0.0) + (model * vec4(position, 1.0));
   gl_Position = projection * view * worldPos;

   v_TexCoords = uv;
   v_Normal = normalMatrix * normal;
   v_FragPos = worldPos.xyz;
}
