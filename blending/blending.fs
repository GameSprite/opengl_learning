#version 330 core
uniform sampler2D texture1;
in vec2 TexCoods;

out vec4 fragColor;
void main()
{
    vec4 texColor = texture(texture1,TexCoods);
    fragColor = texColor;
}