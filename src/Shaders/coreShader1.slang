struct Vertices
{
    float px, py, pz;
    uint8_t tx, ty, tz, tw;
    uint8_t nx, ny, nz, nw;
    half tu, tv;
};

struct ModelPosition
{
    float4x4 pos;
    float4 colour;
    float padd[12];
};

struct SceneData
{
    Vertices* vertexData;
    ModelPosition* modelPositions;
    uint index;
};

struct LocalConstants
{
    float4x4 globalModel;
    float4x4 viewPerspective;
    float4 eye;
    float4 light;
}; 

struct MaterialConstant
{
    float4x4 model;
    float4x4 modelInv;
    
    uint4 textures;
    float4 baseColourFactor;
    float4 metallicRoughnessOcclusionFactor;
    float alphaCutoff;
    
    float3 emissiveFactor;
    uint emissiveTextureIndex;
    uint flags;
};

[[vk::binding(0, 0)]] ConstantBuffer<LocalConstants> localConstants;

[[vk::binding(1, 0)]] ConstantBuffer<MaterialConstant> materialConstant;

[[vk::binding(0, 1)]] __DynamicResource<__DynamicResourceKind.General> globalTextures[];
//Write only image do not need formatting in layout.
[[vk::binding(1, 1)]] __DynamicResource<__DynamicResourceKind.General> globalImages2D[];


struct VSOutput
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord0 : TEXCOORD0;
    float3 vNormal;
    float4 vTangent;
    float4 vColour;
};

[[vk::push_constant]]
cbuffer PushConstantsBuffer
{
    SceneData data;
};

[shader("vertex")]
VSOutput main(uint indinceIndex : SV_VulkanVertexID)
{
	VSOutput output;

	float3 position = float3(data.vertexData[indinceIndex].px, 
                         data.vertexData[indinceIndex].py, 
                         data.vertexData[indinceIndex].pz);
    
    float4 tagent = float4(int(data.vertexData[indinceIndex].tx), 
                       int(data.vertexData[indinceIndex].ty), 
                       int(data.vertexData[indinceIndex].tz), 
                       int(data.vertexData[indinceIndex].tw)) / 127.f - 1.0;
    
    float3 normal = float3(int(data.vertexData[indinceIndex].nx), 
                       int(data.vertexData[indinceIndex].ny),
                       int(data.vertexData[indinceIndex].nz)) / 127.f - 1.0;
    
    float2 texcoord = float2(data.vertexData[indinceIndex].tu, data.vertexData[indinceIndex].tv);

    output.vPosition = mul(mul(mul(mul(localConstants.viewPerspective, localConstants.globalModel), materialConstant.model), data.modelPositions[data.index].pos), float4(position, 1.0));

    output.vTexcoord0 = texcoord;
    output.vNormal = mul(float3x3(materialConstant.modelInv), normal);

    output.vTangent = tagent;
    output.vColour = data.modelPositions[data.index].colour;

	return output;
}

[shader("fragment")]
float4 main(VSOutput input)
{	
	float4 fragColour = globalTextures[NonUniformResourceIndex(materialConstant.textures.x)].asOpaqueDescriptor<Sampler2D>().Sample(input.vTexcoord0) * materialConstant.baseColourFactor;
	fragColour *= input.vColour;
	
	return fragColour;
}
