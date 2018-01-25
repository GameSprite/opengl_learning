#version 330 core
in vec2 texCoords;
uniform sampler2D texture1;
out vec4 color;
void main()
{
    color = vec4(vec3(1.0 - texture(texture1,texCoords)),1.0);
}