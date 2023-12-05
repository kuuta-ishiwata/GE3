struct TransformationMatrix
{
   
    float4x4 WVP;

};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);


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
    output.positon = mul(input.position, gTransformationMatrix.WVP);
    return output;
}