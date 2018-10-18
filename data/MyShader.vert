

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexTexture;

//out vec3 fragmentColor;

uniform highp mat4 projectionMatrix;
uniform highp mat4 transformationMatrix;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

/* Needs to be last because it uses locations 9 to 9 + LIGHT_COUNT - 1 */
#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 9)
#endif
uniform highp vec3 lightPositions[LIGHT_COUNT]; /* defaults to zero */

out mediump vec3 transformedNormal;
out highp vec3 lightDirections[LIGHT_COUNT];
out highp vec3 cameraDirection;

out vec2 interpolatedVertexTexture;

void main() {
     interpolatedVertexTexture = vertexTexture;

     /* Transformed vertex position */
    highp vec4 transformedPosition4 = transformationMatrix*vertexPosition;
    highp vec3 transformedPosition = transformedPosition4.xyz/transformedPosition4.w;

    /* Transformed normal vector */
    transformedNormal = normalMatrix*vertexNormal;

    // Direction to the light
    for(int i = 0; i < LIGHT_COUNT; ++i)
        lightDirections[i] = normalize(lightPositions[i] - transformedPosition);

    /* Direction to the camera */
    cameraDirection = -transformedPosition;

    /* Transform the position */
    gl_Position = projectionMatrix*transformedPosition4;

    gl_PointSize = 10.0;

     //fragmentColor = vertexColor;
     //fragmentColor = vec3(0.2, 0.2, 0.4);

    //fragmentColor = vertexColor * vec3(0.2, 0.2, 0.4);
}
