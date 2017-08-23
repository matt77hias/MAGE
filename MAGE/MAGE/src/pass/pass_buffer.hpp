#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	struct PassBuffer final {

	public:

		explicit PassBuffer(const Scene *scene);
		PassBuffer(const PassBuffer &pass_buffer) = default;
		PassBuffer(PassBuffer &&pass_buffer) = default;
		~PassBuffer() = default;

		PassBuffer &operator=(const PassBuffer &pass_buffer) = delete;
		PassBuffer &operator=(PassBuffer &&pass_buffer) = delete;

		vector< const CameraNode * >			m_cameras;
		vector< const ModelNode * >				m_opaque_models;
		vector< const ModelNode * >				m_transparent_models;
		vector< const DirectionalLightNode * >	m_directional_lights;
		vector< const OmniLightNode * >			m_omni_lights;
		vector< const SpotLightNode * >			m_spot_lights;
		vector< const SpriteNode * >			m_sprites;
		RGBSpectrum								m_ambient_light;
		const SceneFog *						m_fog;
	};
}