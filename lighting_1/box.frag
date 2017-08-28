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
    vec3 deffuse = diff * lightColor;

	float ambientStrength = 0.5f;//环境光照因子
	vec3 ambient = ambientStrength * lightColor;//环境光的颜色

    //color = vec4((ambient + deffuse) * objectColor,1.0f);
	if(diff == 0.0){
	  color = vec4(0.0,1.0,0.0,1.0);
	}else{
	  color = vec4(diff * lightColor,1.0f);
	}
}
