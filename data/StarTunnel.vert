varying vec2 vUv; 

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;

out vec4 fragColor;

uniform highp mat4 projectionMatrix;
uniform highp mat4 transformationMatrix;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform highp vec2 iResolution;
uniform highp float iTime;

out mediump vec3 transformedNormal;

void main()
{
    /* Transformed vertex position */
    highp vec4 transformedPosition4 = transformationMatrix*vertexPosition;
    highp vec3 transformedPosition = transformedPosition4.xyz/transformedPosition4.w;

    /* Transformed normal vector */
    transformedNormal = normalMatrix*vertexNormal;

    /* Transform the position */
    gl_Position = projectionMatrix*transformedPosition4;

}