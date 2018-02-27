#version 330 core
in vec3 Normal;
in vec3 Position;

out vec4 color;

uniform samplerCube tex0;
uniform vec3 cameraPos;
void main()
{
    vec3 viewDir = normalize(Position - cameraPos);
    vec3 reflectDir = reflect(viewDir,normalize(Normal));
    color = texture(tex0,reflectDir);
}