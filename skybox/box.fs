#version 330 core
in vec2 Texcoords;
out vec4 color;
uniform sampler2D tex0;
void main()
{
    color = texture(tex0,Texcoords);
}