//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "camera\viewport.hpp"
#include "rendering\display_configuration.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	const D3D11_VIEWPORT Viewport::GetMaxViewport() noexcept {
		const DisplayConfiguration * const config = DisplayConfiguration::Get();
		
		return GetMaxViewport(config->GetDisplayWidth(), 
			                  config->GetDisplayHeight());
	}
}