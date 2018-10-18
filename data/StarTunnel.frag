#define M_PI 3.1415926535897932384626433832795

uniform vec2     iResolution;
uniform float     iTime;                 // shader playback time (in seconds)

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

out vec4 fragColor;

void main() {

	float size = 30.0;
	float prob = 0.95;
	
	vec2 pos = floor(1.0 / size * gl_FragCoord.xy);
	
	float color = 0.0;
	float starValue = rand(pos);
	
	if (starValue > prob)
	{
		vec2 center = size * pos + vec2(size, size) * 0.5;
		
		float t = 0.9 + 0.2 * sin(iTime + (starValue - prob) / (1.0 - prob) * 45.0);
				
		color = 1.0 - distance(gl_FragCoord.xy, center) / (0.5 * size);
		color = color * t / (abs(gl_FragCoord.y - center.y)) * t / (abs(gl_FragCoord.x - center.x));
	}
	else if (rand(gl_FragCoord.xy / iResolution.xy) > 0.996)
	{
		float r = rand(gl_FragCoord.xy);
		color = r * (0.25 * sin(iTime * (r * 5.0) + 720.0 * r) + 0.75);
	}

#define r(o) fract(sin(vec4(6,9,1,o)*i) * 9e2)
	fragColor = vec4(vec3(color), 1.0);

}