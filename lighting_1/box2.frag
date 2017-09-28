#version 330 core
struct Material
{
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;//高光值
};
out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//观察者的位置坐标
uniform Material material;
void main()
{
	//环境光
	vec3 ambient = lightColor * material.ambient;

	//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = lightColor * material.diffuse * diff;

	//镜面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = lightColor * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	color = vec4(diffuse,1.0);
}
