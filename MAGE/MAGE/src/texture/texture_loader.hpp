#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "platform\windows.hpp"
#include "rendering\rendering_device.hpp"
#include "string\string.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Imports the texture from the given file.

	 @param[in]		fname
					A reference to the file name.
	 @param[in]		device
					A reference to the rendering device.
	 @param[out]	material_buffer
					A pointer to a pointer to a shader resource view.
	 @return		A success/error value.
	 */
	HRESULT ImportTextureFromFile(const wstring &fname, const RenderingDevice &device, ID3D11ShaderResourceView **texture_resource_view);
}