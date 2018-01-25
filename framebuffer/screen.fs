#version 330 core
in vec2 texCoords;
uniform sampler2D texture1;
out vec4 color;
void main()
{
    color = texture(texture1,texCoords);
    float average = (color.r + color.g + color.b) / 3.0;
    color = vec4(average,average,average,1.0);
}