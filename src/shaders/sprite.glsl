@module sprite

@ctype mat4 CF_Matrix4x4
@ctype vec4 CF_Color
@ctype vec2 CF_V2

@block shader_block
vec4 shader(vec4 color, vec2 uv, vec2 pos, vec2 posH, vec4 params)
{
	return color;
}
@end

@include ../../include/shaders/draw.glsl
