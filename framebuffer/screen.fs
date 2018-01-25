#version 330 core
in vec2 texCoords;
uniform sampler2D texture1;
out vec4 color;
void main()
{
    color = texture(texture1,texCoords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average,average,average,1.0);
}