#version 330

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture; //�����������λ��ֵ
uniform sampler2D outTexture2;
uniform float mixvalue;
void main()
{
	//color = texture(ourTexture, TexCoord) * vec4(ourColor,1.0f); // * vec4(ourColor,1.0f)����Ի�ϲ���Ķ�����ɫ
	//ʹ��2�����������
	color = mix(texture(ourTexture, TexCoord), texture(outTexture2,TexCoord),mixvalue); //mixvalue�ǵڶ���������ɫռ�ݵİٷֱ�
}