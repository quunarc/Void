#ifndef RENDER_2D_HDR
#define RENDER_2D_HDR

#include "GPUDevice.hpp"
#include "GPUResources.hpp"
#include "CommandBuffer.hpp"

#include "cglm/struct/mat3.h"
#include "cglm/struct/mat4.h"
#include "cglm/struct/quat.h"
#include "cglm/struct/affine.h"

#include "Foundation/File.hpp"
#include "Foundation/Numerics.hpp"
#include "Foundation/Array.hpp"

struct Render2D
{
	void init(GPUDevice& gpu);
	void drawQuad();

	PipelineHandle pipeline2D;
	BufferHandle vertexBDAHandle;
};

#endif // !RENDER_2D_HDR