struct VSOutput
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};


float4 PS(VSOutput output) : SV_TARGET
{
    return output.color;
}