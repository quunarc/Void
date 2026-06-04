#include "Render2D.hpp"

#include <meshoptimizer.h>

namespace
{
    struct VertexAttributes
    {
        float px, py, pz, padd;
        uint16_t tu, tv;
    };

    struct QuadPositionData
    {
        mat4 position;
        vec4 colour;
        float padd[4];
    };
    /*
        We need track
        instances;
        indices += 6 * instances;
        QuadPositionData.
    */

    static constexpr uint32_t sTotalVertices = 4;
}

void Render2D::init(GPUDevice& gpu)
{
    //Debug renderer
    PipelineCreation debugPipelineCreation{};
    debugPipelineCreation.depthStencil.setDepth(true, VK_COMPARE_OP_GREATER_OR_EQUAL);

    //Shader state
    FileReadResult vert2D = fileReadBinary("Assets/Shaders/2DShader.vert.spv", &MemoryService::instance()->scratchAllocator);
    FileReadResult frag2D = fileReadBinary("Assets/Shaders/2DShader.frag.spv", &MemoryService::instance()->scratchAllocator);

    debugPipelineCreation.shaders.setName("2DRenderPipeline")
        .addStage(vert2D.data, uint32_t(vert2D.size), VK_SHADER_STAGE_VERTEX_BIT)
        .addStage(frag2D.data, uint32_t(frag2D.size), VK_SHADER_STAGE_FRAGMENT_BIT)
        .setSPVInput(true);

    pipeline2D = gpu.createPipeline(debugPipelineCreation);

    Array<VertexAttributes> vertices;
    vertices.init(&MemoryService::instance()->scratchAllocator, sTotalVertices, sTotalVertices);

    //We are assuming the values of 1.f and 0.f are both 1 and 0 respectively for "half" that we are storing in uint16_t
    vertices[0] = { .px = -0.5f, .py = -0.5f, .pz = 1.f, .padd = 0.f, .tu = 0,                         .tv = 0 };
    vertices[1] = { .px =  0.5f, .py = -0.5f, .pz = 1.f, .padd = 0.f, .tu = meshopt_quantizeHalf(1.f), .tv = 0 };
    vertices[2] = { .px =  0.5f, .py =  0.5f, .pz = 1.f, .padd = 0.f, .tu = meshopt_quantizeHalf(1.f), .tv = meshopt_quantizeHalf(1.f) };
    vertices[3] = { .px = -0.5f, .py =  0.5f, .pz = 1.f, .padd = 0.f, .tu = 0,                         .tv = meshopt_quantizeHalf(1.f) };

    BufferCreation bufferCreation{};
    bufferCreation.reset()
        .set(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, sizeof(VertexAttributes) * vertices.size)
        .setName("debugRenderer")
        .setData(vertices.data);
    vertexBDAHandle = gpu.createBindlessBuffer(bufferCreation);
}

void Render2D::drawQuad() 
{
    
}