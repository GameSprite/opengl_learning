#version 330 

out vec4 color;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.1f;//环境光照因子
	vec3 ambient = ambientStrength * lightColor;//环境光的颜色

	vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result,1.0f);
}
