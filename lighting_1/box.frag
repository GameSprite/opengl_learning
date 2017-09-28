#version 330 

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//观察者的位置坐标

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);//漫反射因子
    vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.1f;//环境光照因子
	vec3 ambient = ambientStrength * lightColor;//环境光的颜色

	float specularStrength = 0.5f;//镜面光照强度
	vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量 反
	vec3 reflectDir = reflect(-lightDir,norm);//反射光线方向

	float spec = pow(max(dot(reflectDir,viewDir),0),32);//获取镜面亮度分量,32是物体的发光值，发光值越高，越接近镜子
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result,1.0f);
}
