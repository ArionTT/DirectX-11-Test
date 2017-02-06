

cbuffer GridParameter:register(b0)
{
	float4 GridColor;
	float4 PlaneColor;
	float LineWidth;
	float LineDistance;
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 localPos : TEXCOORD0;
};


float4  pixel(PS_INPUT i):SV_Target
{
	float4 outColor;
	float n = LineWidth;
	float INV_DIST = 1 / LineDistance;
	float z = abs(i.localPos.z);
	float x = abs(i.localPos.x);
	float dotz = frac(z*INV_DIST);
	float dotx = frac(x*INV_DIST);
	if (0 <= dotz&&dotz <= n || 0 <= dotx&&dotx <= n)
		outColor = GridColor;
	else
		outColor = PlaneColor;

	return outColor;
}