#version 460

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outColor;

uniform sampler2D texture_diffuse1;

struct DirectionalLight
{
	vec3 direction;
	vec3 diffuseColor;
	vec3 specColor;
};

uniform vec3 uCameraPos;
uniform float uSpecPower = 32;
uniform vec3 uAmbientLight = vec3(0.2f, 0.2f, 0.2f);
uniform DirectionalLight uDirLight;

void main()
{
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-uDirLight.direction);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.diffuseColor * NdotL;
		vec3 Specular = uDirLight.specColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

    outColor = texture(texture_diffuse1, fragTexCoord) * vec4(Phong, 1.0f);
}
