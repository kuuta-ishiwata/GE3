struct VeRtexShaderOutput
{
    
    float4 positon : SV_POSITION;
    
};


struct VertexShaderInput
{
    
    float4 position : POSITION0;
    
};

VeRtexShaderOutput main(VertexShaderInput input)
{
    
    VeRtexShaderOutput output;
    output.positon = input.position;
    return output;
}