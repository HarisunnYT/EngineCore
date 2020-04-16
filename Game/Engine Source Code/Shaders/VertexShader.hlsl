struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 color : COLOR)
{
	VSOut o;
	o.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	o.color = color;

	return o;
}