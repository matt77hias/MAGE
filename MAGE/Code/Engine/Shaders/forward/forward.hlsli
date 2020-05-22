//-----------------------------------------------------------------------------
// Engine Configuration
//-----------------------------------------------------------------------------
// Defines			                        | Default
//-----------------------------------------------------------------------------
// BRDF_DOT_EPSILON                         | 1e-5f
// BRDF_D_FUNCTION                          | D_GGX
// BRDF_F_FUNCTION                          | F_Schlick
// BRDF_FUNCTION                            | not defined
// BRDF_MINIMUM_ALPHA                       | 1e-1f
// BRDF_V_FUNCTION                          | G_GXX
// DISABLE_BRDF_DIFFUSE                     | not defined
// DISABLE_BRDF_SPECULAR                    | not defined
// DISABLE_FOG                              | not defined
// DISABLE_LIGHTS_AMBIENT                   | not defined
// DISABLE_LIGHTS_DIRECTIONAL               | not defined
// DISABLE_LIGHTS_OMNI                      | not defined
// DISABLE_LIGHTS_SPOT                      | not defined
// DISABLE_LIGHTS_SHADOW_MAPPED             | not defined
// DISABLE_LIGHTS_SHADOW_MAPPED_DIRECTIONAL | not defined
// DISABLE_LIGHTS_SHADOW_MAPPED_OMNI        | not defined
// DISABLE_LIGHTS_SHADOW_MAPPED_SPOT        | not defined
// DISABLE_TEXTURE_BASE_COLOR               | not defined
// DISABLE_TEXTURE_MATERIAL                 | not defined
// DISABLE_TSNM                             | not defined
// DISABLE_VCT                              | not defined
// ENABLE_TRANSPARENCY                      | not defined
// MSAA_AS_SSAA                             | not defined

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "forward\forward_input.hlsli"
#include "lighting.hlsli"

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
#ifdef MSAA_AS_SSAA
OMInputForward PS(PSInputPositionNormalTexture input,
				  uint index : SV_SampleIndex) {
#else  // MSAA_AS_SSAA
OMInputForward PS(PSInputPositionNormalTexture input) {
#endif // MSAA_AS_SSAA

	// Obtain the base color of the material.
	const float4 base_color = GetMaterialBaseColor(input.tex_material);

	#ifdef ENABLE_TRANSPARENCY
	clip(base_color.w - TRANSPARENCY_SKIP_THRESHOLD);
	#else  // ENABLE_TRANSPARENCY
	clip(base_color.w - TRANSPARENCY_THRESHOLD);
	#endif // ENABLE_TRANSPARENCY

	// Obtain the material parameters [roughness, metalness] of the material.
	const float2 material_params = GetMaterialParameters(input.tex_material);
	// Obtain the surface normal expressed in world space.
	const float3 n_world = GetNormal(input.p_world, input.n_world,
									 input.tex_geometry);

	const Material material = {
		base_color.xyz,
		material_params.x,
		material_params.y
	};

	// Calculate the pixel radiance.
	const float3 L = GetRadiance(input.p_world, n_world, material);

	OMInputForward output;
	// Store the radiance.
	output.color = float4(L, base_color.w);
	#pragma warning( push )
	#pragma warning( disable : 3578 ) // Partial initialization.
	// Pack and store the normal.
	output.n.xy  = NORMAL_ENCODE_FUNCTION(n_world);
	#pragma warning( pop )

	return output;
}