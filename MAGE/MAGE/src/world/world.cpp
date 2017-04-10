//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "model\model.hpp"
#include "font\sprite_font.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	World::World() 
		: m_models(), m_lights(), m_sprite_batch(CreateSpriteBatch()) {}

	void World::Render2D() const {
		m_sprite_batch->Begin();
		m_font->DrawString(*m_sprite_batch, L"Hello World", SpriteTransform());
		m_sprite_batch->End();
	}
	void World::Render3D(const TransformBuffer &transform_buffer) const {
		ForEachModel([&](const Model &model) {
			model.Draw(*this, transform_buffer);
		});
	}

	//-------------------------------------------------------------------------
	// Models
	//-------------------------------------------------------------------------

	SharedPtr< Model > World::GetModel(const string &name) const {
		for (vector< SharedPtr< Model > >::const_iterator it = m_models.cbegin(); it != m_models.cend(); ++it) {
			if ((*it)->GetName() == name) {
				return (*it);
			}
		}
		return nullptr;
	}
	bool World::HasModel(const SharedPtr< Model > model) const {
		for (vector< SharedPtr< Model > >::const_iterator it = m_models.cbegin(); it != m_models.cend(); ++it) {
			if ((*it) == model) {
				return true;
			}
		}
		return false;
	}
	void World::RemoveModel(const string &name) {
		vector< SharedPtr< Model > >::iterator it = m_models.begin();
		while (it != m_models.end()) {
			if ((*it)->GetName() == name) {
				it = m_models.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
	void World::RemoveModel(SharedPtr< Model > model) {
		vector< SharedPtr< Model > >::iterator it = m_models.begin();
		while (it != m_models.end()) {
			if ((*it) == model) {
				it = m_models.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}

	//-------------------------------------------------------------------------
	// Lights
	//-------------------------------------------------------------------------

	SharedPtr< PointLight > World::GetLight(const string &name) const {
		for (vector< SharedPtr< PointLight > >::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) {
			if ((*it)->GetName() == name) {
				return (*it);
			}
		}
		return nullptr;
	}
	bool World::HasLight(const SharedPtr< PointLight > light) const {
		for (vector< SharedPtr< PointLight > >::const_iterator it = m_lights.cbegin(); it != m_lights.cend(); ++it) {
			if ((*it) == light) {
				return true;
			}
		}
		return false;
	}
	void World::RemoveLight(const string &name) {
		vector< SharedPtr< PointLight > >::iterator it = m_lights.begin();
		while (it != m_lights.end()) {
			if ((*it)->GetName() == name) {
				it = m_lights.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
	void World::RemoveLight(SharedPtr< PointLight > light) {
		vector< SharedPtr< PointLight > >::iterator it = m_lights.begin();
		while (it != m_lights.end()) {
			if ((*it) == light) {
				it = m_lights.erase(it);
				break;
			}
			else {
				++it;
			}
		}
	}
}