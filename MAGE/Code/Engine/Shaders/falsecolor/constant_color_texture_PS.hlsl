//-----------------------------------------------------------------------------
// Engine Configuration
//-----------------------------------------------------------------------------
// Defines			                        | Default
//-----------------------------------------------------------------------------
// MSAA_AS_SSAA                             | not defined

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "global.hlsli"

//-----------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------
CBUFFER(Color, SLOT_CBUFFER_COLOR) {

	/**
	 The (linear) color.
	 */
	float4 g_color : packoffset(c0);
};

//-----------------------------------------------------------------------------
// SRVs
//-----------------------------------------------------------------------------
TEXTURE_2D(g_texture, float4, SLOT_SRV_TEXTURE);

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
#ifdef MSAA_AS_SSAA
float4 PS(PSInputPositionNormalTexture input,
		  uint index : SV_SampleIndex) : SV_Target {
#else  // MSAA_AS_SSAA
float4 PS(PSInputPositionNormalTexture input) : SV_Target {
#endif // MSAA_AS_SSAA

	return g_color * g_texture.Sample(g_linear_wrap_sampler, input.tex_material);
}