#version 330 core
in vec2 texCoords;
uniform sampler2D texture1;
out vec4 color;
float offset = 1.0/300;//kernel¾àÖÐÐÄÆ«ÒÆ
vec2 offsets[9] = vec2[](
    vec2(-offset,offset),//left-top
    vec2(0.0,offset),     //center-top
    vec2(offset,offset),  //right-top
    vec2(-offset,0.0),    //left-center
    vec2(0.0,0.0),        //center-center
    vec2(offset,0.0),     //right-center
    vec2(-offset,-offset),//left-bottom
    vec2(0.0,-offset),    //center-bottom
    vec2(offset,-offset)  //right-bottom
);
void main()
{
    float kernel[9] = float[](
        -1,-1,-1,
        -1,9,-1,
        -1,-1,-1
    );

    vec3 col;
    for(int i = 0;i < 9;i++){
        vec3 resElement = kernel[i] * vec3(texture(texture1,texCoords.st + offsets[i]));
        col += resElement;
    }
    color = vec4(col,1.0);
}