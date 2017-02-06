
cbuffer ConstantBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VS_INPUT
{
    float4 position:POSITION;
    float3 normal:NORMAL;
    float2 texcoord:TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD0;
};



PS_INPUT  vert(VS_INPUT i)
{
    PS_INPUT o;
    o.Pos=mul(world,i.position);
    o.Pos=mul(view,o.Pos);
    o.Pos=mul(projection,o.Pos);
	o.Normal = mul(world, i.normal);
    return o;
}