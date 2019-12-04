#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Data
{
	vec2 texCoord;
vec3 position;
vec3 normal;
vec3 light;

} data_in[];

out Data
{
	vec2 texCoord;
vec3 position;
vec3 normal;
vec3 light;

} data_out;

uniform int change_shader;

void main()
{
    // If FLAT SHADER, then recalculate normals here
	if (change_shader == 0) {
	    data_out.position = data_in[0].position;
		data_out.texCoord = data_in[0].texCoord;
		gl_Position = gl_in[0].gl_Position;
		data_out.normal = normalize(cross(data_in[1].position - data_in[0].position, data_in[2].position - data_in[0].position));
		data_out.light = data_in[0].light;
		
		EmitVertex();
		data_out.position = data_in[1].position;
		data_out.light = data_in[1].light;

		data_out.texCoord = data_in[1].texCoord;
		gl_Position = gl_in[1].gl_Position;
		
		EmitVertex();
		data_out.position = data_in[2].position;
		data_out.light = data_in[2].light;

		data_out.texCoord = data_in[2].texCoord;
		gl_Position = gl_in[2].gl_Position;
		
		EmitVertex();

		EndPrimitive();
	}

	// If using any other shader, then just pass the information as given in the vertex shader
	else {
	     data_out.position = data_in[0].position;
		data_out.normal = data_in[0].normal;
		
	    data_out.texCoord = data_in[0].texCoord;
		data_out.light = data_in[0].light;
		gl_Position = gl_in[0].gl_Position;
		
		EmitVertex();
		 data_out.position = data_in[1].position;
		data_out.light = data_in[1].light;
		data_out.normal = data_in[1].normal;

		data_out.texCoord = data_in[1].texCoord;
		gl_Position = gl_in[1].gl_Position;
		
		EmitVertex();
		 data_out.position = data_in[2].position;
		data_out.normal = data_in[2].normal;
		data_out.light = data_in[2].light;

		data_out.texCoord = data_in[2].texCoord;
		gl_Position = gl_in[2].gl_Position;
		
		EmitVertex();

		EndPrimitive();

	}
	
	
}