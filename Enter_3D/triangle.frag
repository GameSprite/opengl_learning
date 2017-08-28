#version 330

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture; //纹理采样器的位置值
uniform sampler2D outTexture2;
uniform float mixvalue;
void main()
{
	//color = texture(ourTexture, TexCoord) * vec4(ourColor,1.0f); // * vec4(ourColor,1.0f)后可以混合插入的顶点颜色
	//使用2个纹理采样器
	color = mix(texture(ourTexture, TexCoord), texture(outTexture2,TexCoord),mixvalue); //mixvalue是第二个纹理颜色占据的百分比
}