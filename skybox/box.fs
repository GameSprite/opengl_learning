#version 330 core
in vec3 Normal;
in vec3 Position;

out vec4 color;

uniform samplerCube tex0;
uniform vec3 cameraPos;
void main()
{
    //折射指数的比率
    float ratio = 1.0 / 1.52;//从空气射入玻璃
    vec3 viewDir = normalize(Position - cameraPos);
    vec3 refractDir = refract(viewDir,normalize(Normal),ratio);//折射光线，也就是眼睛透过玻璃箱子，看到的天空盒的纹理坐标
    color = texture(tex0,refractDir);
}