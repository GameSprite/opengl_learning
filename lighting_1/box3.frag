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
	vec3 position;//手电的位置
	vec3 direction;//手电的照射方向
	float cutoff;//聚光的切光角的余弦值

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//3个衰减参数
	float constant;//常数项
	float linear;//1次项
	float quadratic;//2次项
};
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightColor; //不再使用，使用Light代替单一的光源颜色
uniform vec3 viewPos;//观察者的位置坐标
uniform Material material;
uniform Light light;

out vec4 color;
void main()
{
	//光线方向(由片段指向光源)
	vec3 lightVec = light.position - FragPos;
	vec3 lightDir = normalize(lightVec);
	//环境光
	vec3 ambient = light.ambient * material.ambient;

	//计算是否在聚光范围内
	float theta = dot(lightDir,normalize(-light.direction));
	if(theta > light.cutoff){
		//计算光照
		//漫反射
		vec3 norm = normalize(Normal);
		float diff = max(dot(lightDir,norm),0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
		//镜面反射
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
		vec3 specular = light.specular * (spec * vec3(texture(material.specular,TexCoords)));
		//点光源光照强度衰减
		float distance = length(lightVec);
		float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic*(distance*distance));
		color = vec4((ambient + diffuse + specular)*attenuation,1.0);
	}else{
		//使用环境光和漫反射纹理
		color = vec4(light.ambient*vec3(texture(material.diffuse,TexCoords)),1.0f);
	}
	
}