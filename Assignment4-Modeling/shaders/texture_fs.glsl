#version 330 core
out vec4 fragColor;

in Data {
	vec2 texCoord;
 vec3 position;
	vec3 normal;
	vec3 light;
} data;
uniform vec3 lightPosition;
uniform int change_shader;
uniform sampler2D tex;
uniform vec3 lightColor;
//uniform int textured;

void main()

{	

  

	//vec3 c = texture(tex, data.texCoord).rgb;
	
	//if(!textured){
	 vec3 c = vec3 (1,0,0);
	//}
	
	//If doing FLAT SHADING OR PHONG SHADING do lighting calculation here
	if (change_shader == 0 || change_shader==2) {
		vec3 lightposition = vec3(10.f, 10.f, 10.f);

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

		vec3 result = (ambient + diffuse + specular) * c;
		fragColor = vec4(result, 1.0);
	}
	// For Gouraud Shading just pass lighting values
	else {
		fragColor = vec4(data.light*c, 1.0);
	}
  
}