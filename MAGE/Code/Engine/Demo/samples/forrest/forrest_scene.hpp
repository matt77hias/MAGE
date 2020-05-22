#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scene\scene.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Game Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class ForrestScene : public Scene {

	public:

		ForrestScene();

		ForrestScene(const ForrestScene& scene) = delete;

		ForrestScene(ForrestScene&& scene);

		virtual ~ForrestScene();

		ForrestScene& operator=(const ForrestScene& scene) = delete;

		ForrestScene& operator=(ForrestScene&& scene) = delete;

	private:

		virtual void Load([[maybe_unused]] Engine& engine) override;
	};
}