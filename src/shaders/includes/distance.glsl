@block distance

float safe_div(float a, float b)
{
	return b == 0.0 ? 0.0 : a / b;
}

float safe_len(vec2 v)
{
	float d = dot(v,v);
	return d == 0.0 ? 0.0 : sqrt(d);
}

vec2 safe_norm(vec2 v, float l)
{
	return mix(vec2(0), v / l, l == 0.0 ? 0.0 : 1.0);
}

vec2 skew(vec2 v)
{
	return vec2(-v.y, v.x);
}

float det2(vec2 a, vec2 b)
{
	return a.x * b.y - a.y * b.x;
}

float sdf_stroke(float d)
{
	return clamp(abs(d) - v_stroke, -1.0, 1.0);
}

float sdf_intersect(float a, float b)
{
	return max(a, b);
}

float sdf_union(float a, float b)
{
	return min(a, b);
}

float sdf_subtract(float d0, float d1)
{
	return max(d0, -d1);
}

vec4 sdf(vec4 a, vec4 b, float d)
{
	vec4 stroke_aa = mix(a, b, 1.0 - mix(0.0, 1.0, sdf_stroke(d)));
	vec4 stroke_no_aa = sdf_stroke(d) <= 0.0 ? b : a;

	vec4 fill_aa = mix(a, b, 1.0 - mix(0.0, 1.0, clamp(d, 0, 1.0)));
	vec4 fill_no_aa = clamp(d, -1.0, 1.0) <= 0.0 ? b : a;

	vec4 stroke = mix(stroke_no_aa, stroke_aa, v_aa);
	vec4 fill = mix(fill_no_aa, fill_aa, v_aa);

	result = mix(stroke, fill, v_fill);
	return result;
}

//--------------------------------------------------------------------------------------------------

float distance_aabb(vec2 p, vec2 he)
{
	vec2 d = abs(p) - he;
	return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float distance_box(vec2 p, vec2 c, vec2 he, vec2 u)
{
	mat2 m = transpose(mat2(u, skew(u)));
	p = p - c;
	p = m * p;
	return distance_aabb(p, he);
}

// Referenced from: https://www.shadertoy.com/view/3tdSDj
float distance_segment(vec2 p, vec2 a, vec2 b)
{
	vec2 n = b - a;
	vec2 pa = p - a;
	float d = safe_div(dot(pa,n), dot(n,n));
	float h = clamp(d, 0.0, 1.0);
	return safe_len(pa - h * n);
}

float distance_line(vec2 p, vec2 a, vec2 b)
{
	vec2 n = normalize(skew(b - a));
	float d = dot(n, a);
	return abs(dot(p, n) - d);
}

// Referenced from: https://www.shadertoy.com/view/XsXSz4
float distance_triangle(vec2 p, vec2 a, vec2 b, vec2 c)
{
	vec2 e0 = b - a;
	vec2 e1 = c - b;
	vec2 e2 = a - c;

	vec2 v0 = p - a;
	vec2 v1 = p - b;
	vec2 v2 = p - c;

	vec2 pq0 = v0 - e0 * clamp(safe_div(dot(v0, e0), dot(e0, e0)), 0.0, 1.0);
	vec2 pq1 = v1 - e1 * clamp(safe_div(dot(v1, e1), dot(e1, e1)), 0.0, 1.0);
	vec2 pq2 = v2 - e2 * clamp(safe_div(dot(v2, e2), dot(e2, e2)), 0.0, 1.0);

	float s = det2(e0, e2);
	vec2 d = min(min(vec2(dot(pq0, pq0), s * det2(v0, e0)),
	                 vec2(dot(pq1, pq1), s * det2(v1, e1))),
	                 vec2(dot(pq2, pq2), s * det2(v2, e2)));

	return -sqrt(d.x) * sign(d.y);
}

@end