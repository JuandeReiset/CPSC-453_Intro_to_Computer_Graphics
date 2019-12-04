#version 330 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out Data
{
	vec2 texCoord;
vec3 position;
vec3 normal;
vec3 light;

} data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform int change_shader;


void main()
{
	data.texCoord = texCoord;
	data.position = vec3(model * vec4(position, 1.0));
	data.normal = normalize(mat3(model) * normal);

	//If Gouraud Shader, then calculate Lighting here
	if (change_shader == 1) {
		
		//Calculate Ambient
		float d = length(data.position - lightPosition);
		float ambientStrength = 0.2;
		vec3 ambient = ambientStrength * lightColor;

		// Calculate Diffusion
		vec3 n = normalize(data.normal);
		vec3 lightDirection = normalize(lightPosition - data.position);
		float diff = max(dot(n, lightDirection), 0.0);
		vec3 diffuse = diff * lightColor;

		// Calculate Specular
		float specularStrength = 0.5;
		vec3 viewDirection = normalize(vec3(0, 0, 1) - data.position);
		vec3 reflectDirection = reflect(-lightDirection, n);
		float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular);
		data.light = result;
	}
	
    gl_Position = projection * view * model * vec4(position,1.f);
}