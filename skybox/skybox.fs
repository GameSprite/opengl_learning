#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skyboxCubeTexture;
void main()
{
    color = texture(skyboxCubeTexture,TexCoords);
}

                                                  