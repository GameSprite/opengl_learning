#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;//高光值
};
struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight
{
	vec3 position;

    float constant;//衰减常数项系数
    float linear;//衰减线性系数
    float quadratic;//衰减2次项系数

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; 
};
struct SpotLight{
	//点光源属性
	vec3 position;

    float constant;//衰减常数项系数
    float linear;//衰减线性系数
    float quadratic;//衰减2次项系数

	vec3 ambient;
	vec3 diffuse;
	vec3 specular; 
	//聚光属性
	vec3 direction;//手电光方向
	float cutoff;//内圆锥切光角余弦值
	float outCutoff;//外圆锥切光角余弦值
};
#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight; //平行光
uniform PointLight pointLights[NR_POINT_LIGHTS];//点光源数组
uniform SpotLight spotLight;//手电光
vec3 CalcDirLight(DirLight dirLight,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight,vec3 normal, vec3 fragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight,vec3 normal,vec3 fragPos,vec3 viewDir);
void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);//注意和实际是反向的
	//计算平行光照
	vec3 result = CalcDirLight(dirLight,norm,viewDir);
	//计算点光光照
	for(int i=0;i<NR_POINT_LIGHTS;i++)
		result += CalcPointLight(pointLights[i],norm,FragPos,viewDir);
	//计算聚光光照
	result += CalcSpotLight(spotLight,norm,FragPos,viewDir);
	color = vec4(result,1.0f);
}

vec3 CalcDirLight(DirLight dirLight,vec3 normal,vec3 viewDir){
	vec3 lightDir = normalize(-dirLight.direction);
	//计算环境光
	vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoords));
    //计算漫反射光
	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuse = diff * dirLight.diffuse * vec3(texture(material.diffuse,TexCoords));
	//计算镜面反射光
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = spec * dirLight.specular * vec3(texture(material.specular,TexCoords));
	//合并光照分量
	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight pointLight,vec3 normal, vec3 fragPos,vec3 viewDir){
	float dis = length(fragPos - pointLight.position);
	float attenuation = 1.0/(pointLight.constant + pointLight.linear * dis + pointLight.quadratic * dis * dis);

    vec3 lightDir = normalize(pointLight.position - fragPos);

	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse,TexCoords));
	ambient *= attenuation;

	float diff = max(dot(lightDir,normal),0.0);
	vec3 diffuse = diff * pointLight.diffuse * vec3(texture(material.diffuse,TexCoords));
	diffuse *= attenuation;

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
	vec3 specular = spec * pointLight.specular * vec3(texture(material.specular,TexCoords));
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight spotLight,vec3 normal,vec3 fragPos,vec3 viewDir){
	vec3 direction = normalize(-spotLight.direction);
	vec3 lightDir = normalize(spotLight.position - fragPos);

	float cos_theta = dot(direction,lightDir);
	if(cos_theta < spotLight.outCutoff){
		vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse,TexCoords));

		float diff = max(dot(normal,lightDir),0.0);
		vec3 diffuse = diff * spotLight.diffuse * vec3(texture(material.diffuse,TexCoords));

		vec3 reflectDir = reflect(-lightDir,normal);
		float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
		vec3 specular = spec * spotLight.specular * vec3(texture(material.specular,TexCoords));
		//强度衰减
		float dis = length(spotLight.position - fragPos);
		float attenuation = 1.0/(spotLight.constant + spotLight.linear * dis + spotLight.quadratic * dis * dis);
		//边缘软化
		float epsilon = spotLight.cutoff - spotLight.outCutoff;
		float intensity = clamp((cos_theta - spotLight.outCutoff)/epsilon,0.0,1.0);

		diffuse *= intensity;
		specular *= intensity;

		return (ambient + diffuse + specular) * attenuation;

	}else{
		//如果要光找不到的地方全黑的话，其实可以返回(0,0,0)黑色
		return spotLight.ambient * vec3(texture(material.diffuse,TexCoords));
	}
}