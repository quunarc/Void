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

[[vk::binding(0, 1)]] Sampler2D globalTextures[];
//Alias textures to use the same binding point, as bindless texture is shared
//between all kind of textures: 1d, 2d, 3d.
[[vk::binding(0, 1)]] Sampler3D globalTextures3D[];

[[vk::binding(0, 1)]] SamplerCube globalTexturesCube[];

//Write only image do not need formatting in layout.
[[vk::binding(1, 1)]] RWTexture2D<float4> globalImages2D[];

struct VSOutput
{
    float2 vTexcoord0;
    float3 vNormal;
    float4 vTangent;
    float4 vPosition;
    float4 vColour;
};

[shader("fragment")]
float4 main(VSOutput input)
{
    float4 fragColour = globalTextures[NonUniformResourceIndex(materialConstant.textures.x)].Sample(input.vTexcoord0) * materialConstant.baseColourFactor;
    fragColour *= input.vColour;
	
	return fragColour;
}