#pragma once

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	template<>
	[[nodiscard]]
	constexpr DXGI_FORMAT GetIndexFormat< U16 >() noexcept {
		return DXGI_FORMAT_R16_UINT;
	}

	template<>
	[[nodiscard]]
	constexpr DXGI_FORMAT GetIndexFormat< U32 >() noexcept {
		return DXGI_FORMAT_R32_UINT;
	}
}