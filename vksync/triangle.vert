#version 450

layout (location = 0) out vec2 uv;

layout (push_constant) uniform time_constants 
{
	float time;
};

void main()
{
	vec2 P = vec2(gl_VertexIndex & 1, (gl_VertexIndex & 2) >> 1);
	vec2 pos = P * 4 - 1;

	pos *= 0.25;

	gl_Position = vec4(mat2(cos(time), -sin(time), 
					sin(time), cos(time)) * pos.xy, 0.0, 1.0);

	uv = pos;
}
