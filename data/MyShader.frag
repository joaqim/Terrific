
uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform sampler2D textureData;

in vec2 interpolatedVertexTexture;

out vec3 fragmentColor;

void main() {
     //fragmentColor = color;
     fragmentColor.rgb = color*texture(textureData, interpolatedVertexTexture).rgb;
     //fragmentColor.a = 1.0;
}