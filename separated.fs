#version 330

uniform sampler2D textureColor;

uniform vec2 dir;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourColor;

const int M = 16;
const int N = 2 * M + 1;

// sigma = 10
const float coeffs[N] = float[N](
	0.012318109844189502,
	0.014381474814203989,
	0.016623532195728208,
	0.019024086115486723,
	0.02155484948872149,
	0.02417948052890078,
	0.02685404941667096,
	0.0295279624870386,
	0.03214534135442581,
	0.03464682117793548,
	0.0369716985390341,
	0.039060328279673276,
	0.040856643282313365,
	0.04231065439216247,
	0.043380781642569775,
	0.044035873841196206,
	0.04425662519949865,
	0.044035873841196206,
	0.043380781642569775,
	0.04231065439216247,
	0.040856643282313365,
	0.039060328279673276,
	0.0369716985390341,
	0.03464682117793548,
	0.03214534135442581,
	0.0295279624870386,
	0.02685404941667096,
	0.02417948052890078,
	0.02155484948872149,
	0.019024086115486723,
	0.016623532195728208,
	0.014381474814203989,
	0.012318109844189502
);

void main()
{
	vec4 sum = vec4(0.0);
	for (int i = 0; i < N; i++)
	{
		vec2 tc = TexCoord + dir * float(i - M);
		sum += coeffs[i] * texture(textureColor, tc);
	}

	FragColor = sum;
}