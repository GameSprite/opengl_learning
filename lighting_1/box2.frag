#version 330 core
struct Material
{
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;//�߹�ֵ
};
out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//�۲��ߵ�λ������
uniform Material material;
void main()
{
	//������
	vec3 ambient = lightColor * material.ambient;

	//�������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = lightColor * material.diffuse * diff;

	//����߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = lightColor * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	color = vec4(diffuse,1.0);
}
