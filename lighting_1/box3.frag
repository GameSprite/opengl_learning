#version 330
struct Material
{
	vec3  ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;//高光值
};
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//观察者的位置坐标
uniform Material material;
uniform Light light;

out vec4 color;
void main()
{
	//环境光
	vec3 ambient = light.ambient * material.ambient;
	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(lightDir,norm),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular,TexCoords)));

	color = vec4(ambient + diffuse + specular,1.0);
}