//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "renderer\factory.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	//-------------------------------------------------------------------------
	// Blend states
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]]
	HRESULT CreateOpaqueBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula (i.e.no blending) is defined as:
		// (source.rgba × 1) + (destination.rgba × 0) = source.rgba.

		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable           = FALSE;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateAlphaBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula is defined as:
		// (source.rgba × source.a) + (destination.rgba × (1-source.a)).

		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable           = TRUE;
		desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateAdditiveBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula is defined as:
		// (source.rgba × 1) + (destination.rgba × 1).

		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable           = TRUE;
		desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateMultiplicativeBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula is defined as:
		// (source.rgba × 0) + (destination.rgba × source.rgba).

		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable           = TRUE;
		desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend             = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateBiMultiplicativeBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula is defined as:
		// (source.rgba × destination.rgba ) + (destination.rgba × source.rgba).

		D3D11_BLEND_DESC desc = {};
		desc.RenderTarget[0].BlendEnable           = TRUE;
		desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_DEST_COLOR;
		desc.RenderTarget[0].DestBlend             = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_DEST_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateTransparencyBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		// The blend formula for the first RTV is defined as:
		// (source.rgba × source.a) + (destination.rgba × (1-source.a)).
		// The blend formula (i.e. no blending) for the remaining RTVs is defined as:
		// (source.rgba × 1       ) + (destination.rgba × 0           ) = source.rgba.

		D3D11_BLEND_DESC desc = {};
		desc.IndependentBlendEnable                    = TRUE;
		// Blending of the color.
		desc.RenderTarget[0].BlendEnable               = TRUE;
		desc.RenderTarget[0].SrcBlend                  = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend                 = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp                   = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha             = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha            = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha              = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask     = D3D11_COLOR_WRITE_ENABLE_ALL;
		// Blending of the normal and the remaining.
		for (std::size_t i = 1u; i < std::size(desc.RenderTarget); ++i) {
			desc.RenderTarget[i].BlendEnable           = FALSE;
			desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		return device.CreateBlendState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateAlphaToCoverageBlendState(
		ID3D11Device& device, NotNull< ID3D11BlendState** > state) noexcept {

		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable                 = TRUE;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		return device.CreateBlendState(&desc, state);
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Depth stencil states
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]]
	HRESULT CreateDepthNoneDepthStencilState(
		ID3D11Device& device, NotNull< ID3D11DepthStencilState** > state) noexcept {

		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable   = FALSE;
		desc.StencilEnable = FALSE;

		return device.CreateDepthStencilState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateDepthReadWriteDepthStencilState(
		ID3D11Device& device, NotNull< ID3D11DepthStencilState** > state,
		D3D11_COMPARISON_FUNC func) noexcept {

		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable    = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc      = func;
		desc.StencilEnable  = FALSE;

		return device.CreateDepthStencilState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateDepthReadDepthStencilState(
		ID3D11Device& device, NotNull< ID3D11DepthStencilState** > state,
		D3D11_COMPARISON_FUNC func) noexcept {

		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable    = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc      = func;
		desc.StencilEnable  = FALSE;

		return device.CreateDepthStencilState(&desc, state);
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Rasterizer states
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]]
	HRESULT CreateRasterizerState(ID3D11Device& device,
								  NotNull< ID3D11RasterizerState** > state,
								  D3D11_CULL_MODE cull_mode,
								  D3D11_FILL_MODE fill_mode,
								  S32 depth_bias,
								  F32 slope_scaled_depth_bias,
								  F32 depth_bias_clamp) noexcept {

		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode             = cull_mode;
		desc.FillMode             = fill_mode;

		#ifdef DISABLE_INVERTED_Z_BUFFER
		desc.DepthBias            = depth_bias;
		desc.SlopeScaledDepthBias = slope_scaled_depth_bias;
		desc.DepthBiasClamp       = depth_bias_clamp;
		#else  // DISABLE_INVERTED_Z_BUFFER
		desc.DepthBias            = -depth_bias;
		desc.SlopeScaledDepthBias = -slope_scaled_depth_bias;
		desc.DepthBiasClamp       = -depth_bias_clamp;
		#endif // DISABLE_INVERTED_Z_BUFFER

		desc.DepthClipEnable      = TRUE;
		desc.MultisampleEnable    = TRUE;

		return device.CreateRasterizerState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreateCullNoneRasterizerState(ID3D11Device& device,
										  NotNull< ID3D11RasterizerState** > state,
										  S32 depth_bias,
										  F32 slope_scaled_depth_bias,
										  F32 depth_bias_clamp) noexcept {

		return CreateRasterizerState(device,
			                         state,
			                         D3D11_CULL_NONE,
			                         D3D11_FILL_SOLID,
			                         depth_bias,
			                         slope_scaled_depth_bias,
			                         depth_bias_clamp);
	}

	[[nodiscard]]
	HRESULT CreateCullClockwiseRasterizerState(ID3D11Device& device,
											   NotNull< ID3D11RasterizerState** > state,
											   S32 depth_bias,
											   F32 slope_scaled_depth_bias,
											   F32 depth_bias_clamp) noexcept {
		return CreateRasterizerState(device,
			                         state,
			                         D3D11_CULL_FRONT,
			                         D3D11_FILL_SOLID,
			                         depth_bias,
			                         slope_scaled_depth_bias,
			                         depth_bias_clamp);
	}

	[[nodiscard]]
	HRESULT CreateCullCounterClockwiseRasterizerState(ID3D11Device& device,
													  NotNull< ID3D11RasterizerState** > state,
													  S32 depth_bias,
													  F32 slope_scaled_depth_bias,
													  F32 depth_bias_clamp) noexcept {
		return CreateRasterizerState(device,
			                         state,
			                         D3D11_CULL_BACK,
			                         D3D11_FILL_SOLID,
			                         depth_bias,
			                         slope_scaled_depth_bias,
			                         depth_bias_clamp);
	}

	[[nodiscard]]
	HRESULT CreateWireframeRasterizerState(ID3D11Device& device,
										   NotNull< ID3D11RasterizerState** > state,
										   S32 depth_bias,
										   F32 slope_scaled_depth_bias,
										   F32 depth_bias_clamp) noexcept {

		return CreateRasterizerState(device,
			                         state,
			                         D3D11_CULL_NONE,
			                         D3D11_FILL_WIREFRAME,
			                         depth_bias,
			                         slope_scaled_depth_bias,
			                         depth_bias_clamp);
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Sampler states
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]]
	HRESULT CreateSamplerState(ID3D11Device& device,
							   NotNull< ID3D11SamplerState** > state,
							   D3D11_FILTER filter,
							   D3D11_TEXTURE_ADDRESS_MODE address_mode) noexcept {

		D3D11_SAMPLER_DESC desc = {};
		desc.Filter         = filter;
		desc.AddressU       = address_mode;
		desc.AddressV       = address_mode;
		desc.AddressW       = address_mode;
		desc.MaxAnisotropy  = (device.GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1)
			                  ? D3D11_MAX_MAXANISOTROPY : 2u;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MaxLOD         = D3D11_FLOAT32_MAX;

		return device.CreateSamplerState(&desc, state);
	}

	[[nodiscard]]
	HRESULT CreatePointWrapSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_POINT,
			                      D3D11_TEXTURE_ADDRESS_WRAP);
	}

	[[nodiscard]]
	HRESULT CreatePointClampSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_POINT,
			                      D3D11_TEXTURE_ADDRESS_CLAMP);
	}

	[[nodiscard]]
	HRESULT CreatePointMirrorSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_POINT,
			                      D3D11_TEXTURE_ADDRESS_MIRROR);
	}

	[[nodiscard]]
	HRESULT CreateLinearWrapSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			                      D3D11_TEXTURE_ADDRESS_WRAP);
	}

	[[nodiscard]]
	HRESULT CreateLinearClampSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			                      D3D11_TEXTURE_ADDRESS_CLAMP);
	}

	[[nodiscard]]
	HRESULT CreateLinearMirrorSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			                      D3D11_TEXTURE_ADDRESS_MIRROR);
	}

	[[nodiscard]]
	HRESULT CreateAnisotropicWrapSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_ANISOTROPIC,
			                      D3D11_TEXTURE_ADDRESS_WRAP);
	}

	[[nodiscard]]
	HRESULT CreateAnisotropicClampSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_ANISOTROPIC,
			                      D3D11_TEXTURE_ADDRESS_CLAMP);
	}

	[[nodiscard]]
	HRESULT CreateAnisotropicMirrorSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		return CreateSamplerState(device,
			                      state,
			                      D3D11_FILTER_ANISOTROPIC,
			                      D3D11_TEXTURE_ADDRESS_MIRROR);
	}

	[[nodiscard]]
	HRESULT CreatePCFSamplerState(
		ID3D11Device& device, NotNull< ID3D11SamplerState** > state) noexcept {

		D3D11_SAMPLER_DESC desc = {};
		desc.Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		desc.AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW       = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.MaxAnisotropy  = (device.GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1)
			                  ? D3D11_MAX_MAXANISOTROPY : 2u;

		#ifdef DISABLE_INVERTED_Z_BUFFER
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		#else  // DISABLE_INVERTED_Z_BUFFER
		desc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;

		// PCF sampling on shadow map border (i.e. some subsamples will be
		// located outside the shadow map).
		//
		// src >= dst -> src >= 0.0 (far plane)
		// -> succeeds for border (sub)sample -> (sub)sample is not in shadow
		//
		// src >= dst -> src >= 1.0 (near plane)
		// -> fails for border (sub)sample    -> (sub)sample is in shadow
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;

		// Alternative:
		// 1. Generate shadow maps with a larger size (directional lights) or
		//    a larger umbra/FOV_y (spotlights), which corresponds to a
		//    larger light-to-projection frustum. This ensures depth values
		//    will be stored for positions just outside the shadow map borders.
		// 2. Use the original light-to-projection matrix for lighting
		//    computations. This enables smoother and more continuous soft
		//    shadows at the shadow map borders (as opposed to the
		//    discontinuity introduced by the border color).

		#endif // DISABLE_INVERTED_Z_BUFFER

		desc.MaxLOD         = D3D11_FLOAT32_MAX;

		return device.CreateSamplerState(&desc, state);
	}

	#pragma endregion
}