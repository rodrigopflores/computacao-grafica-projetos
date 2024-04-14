#version 460

in vec2 texCoord;
in vec3 fragPos;
in vec3 scaledNormal;

out vec4 color;

uniform sampler2D tex_buffer;

//Propriedades da superficie
uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform float q;

//Propriedades da fonte de luz
uniform vec3 lightPos;
uniform vec3 lightColor;

//Posição da Camera
uniform vec3 cameraPos;

void main()
{
	//Cálculo da parcela de iluminação ambiente
	vec3 ambient = Ka * lightColor;

	//Cálculo da parcela de iluminação difusa
	vec3 N = normalize(scaledNormal);
	vec3 L = normalize(lightPos - fragPos);
	float diff = max(dot(N,L),0.0);
	vec3 diffuse = Kd * diff * lightColor;

	//Cálculo da parcela de iluminação especular
	vec3 V = normalize(cameraPos - fragPos);
	vec3 R = normalize(reflect(-L,N));
	float spec = max(dot(R,V),0.0);
	spec = pow(spec,q);
	vec3 specular = Ks * spec * lightColor;
	vec3 texColor = texture(tex_buffer, texCoord).rgb;
	vec3 result = (ambient + diffuse) * texColor + specular;

	color = vec4(result,1.0);
}