#version 330 core

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColour;
uniform vec3 u_AmbientStrength;

out vec4 colour;

void main()
{
   vec3 norm = normalize(v_Normal);
   vec3 lightDir = normalize(-u_LightDirection);

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * u_LightColour;

   vec3 ambient = u_AmbientStrength * u_LightColour;

   vec4 texColour = texture(u_Texture, v_TexCoords);

   vec3 result = (ambient + diffuse) * texColour.rgb;

   colour = vec4(result, texColour.a);
}

   // colour = vec4(v_TexCoords, 0.0, 1.0);