#version 330 core
uniform sampler2D texture1;
in vec2 TexCoods;

out vec4 fragColor;
void main()
{
    fragColor = texture(texture1,TexCoods);
}