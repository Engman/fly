#ifndef SHADER_FLAG_STATES_H
#define SHADER_FLAG_STATES_H

/** A collection of states to be set */
namespace FLAGS
{
	enum STATE_RASTERIZER
	{
		RASTERIZER_BackCullNoMS,	
		RASTERIZER_FrontCullNoMS,
		RASTERIZER_NoCullNoMs,
		RASTERIZER_NoCullNoMSWF,
		RASTERIZER_Custom,
	};
	enum STATE_BLEND_MODE
	{
		BLEND_MODE_AlphaBlend,
		BLEND_MODE_DisabledBlend,
		BLEND_MODE_Custom,
	};
	enum STATE_DEPTH_STENCIL
	{
		DEPTH_STENCIL_DisabledDepth,
		DEPTH_STENCIL_EnabledDepth,
		DEPTH_STENCIL_Custom,
	};
	enum STATE_SAMPLING
	{
		SAMPLER_Point,
		SAMPLER_Linear,
		SAMPLER_Anisotropic2,
		SAMPLER_Anisotropic4,
		SAMPLER_Anisotropic8,
		SAMPLER_Anisotropic16,
		SAMPLER_Custom,
		SAMPLER_None,
	};

	enum SHADERS
	{
		VS,
		PS,
		HS,
		GS,
		DS,
	};
}

#endif

