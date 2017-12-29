#version 330 core
uniform sampler2D texture1;
in vec2 TexCoords;
out vec4 FragColor;

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 100.0;
    float z = depth * 2.0 - 1.0; //back to NDC
    return (2.0 * near) / (far + near - z * (far - near));
}
void main()
{
    FragColor = texture(texture1,TexCoords);
    //FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z)),1.0f);
}