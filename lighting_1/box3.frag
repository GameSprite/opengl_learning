#version 330
struct Material
{
	vec3  ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;//高光值
};
struct Light
{
	//vec3 position; //平行光不需要光源位置了，因为是无限远的,只需要一个方向向量就行
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor; //不再使用，使用Light代替单一的光源颜色
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
	//光线方向(由片段指向光源)
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(Normal);
	float diff = max(dot(lightDir,norm),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular,TexCoords)));

	color = vec4(ambient + diffuse + specular,1.0);
}