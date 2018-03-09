#pragma once

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering {

	//-------------------------------------------------------------------------
	// ResourceManager: GetPool
	//-------------------------------------------------------------------------
	#pragma region

	template< typename ResourceT >
	[[nodiscard]]
	inline ResourceManager::pool_type< ResourceT >& 
		ResourceManager::GetPool() noexcept {

		// This member method is non-const 
		// => this object instance is non-const
		// => const_cast is safe.

		return const_cast< pool_type< ResourceT >& >(
			static_cast< const ResourceManager* >(this)->GetPool< ResourceT >());
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< ModelDescriptor >& 
		ResourceManager::GetPool< ModelDescriptor >() const noexcept {

		return m_model_descriptor_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< VertexShader >& 
		ResourceManager::GetPool< VertexShader >() const noexcept {

		return m_vs_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< HullShader >& 
		ResourceManager::GetPool< HullShader >() const noexcept {

		return m_hs_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< DomainShader >& 
		ResourceManager::GetPool< DomainShader >() const noexcept {

		return m_ds_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< GeometryShader >& 
		ResourceManager::GetPool< GeometryShader >() const noexcept {

		return m_gs_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< PixelShader >& 
		ResourceManager::GetPool< PixelShader >() const noexcept {

		return m_ps_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< ComputeShader >& 
		ResourceManager::GetPool< ComputeShader >() const noexcept {

		return m_cs_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< SpriteFont >& 
		ResourceManager::GetPool< SpriteFont >() const noexcept {

		return m_sprite_font_pool;
	}

	template<>
	[[nodiscard]]
	inline const ResourceManager::pool_type< Texture >& 
		ResourceManager::GetPool< Texture >() const noexcept {

		return m_texture_pool;
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// ResourceManager: Remaining
	//-------------------------------------------------------------------------
	#pragma region

	template< typename ResourceT >
	[[nodiscard]]
	inline bool 
		ResourceManager::Contains(const key_type< ResourceT >& guid) noexcept {

		return GetPool< ResourceT >().Contains(guid);
	}

	template< typename ResourceT >
	[[nodiscard]]
	inline SharedPtr< ResourceManager::value_type< ResourceT > >
		ResourceManager::Get(const key_type< ResourceT >& guid) noexcept {

		return GetPool< ResourceT >().Get(guid);
	}

	template< typename ResourceT, typename... ConstructorArgsT >
	SharedPtr< typename ResourceManager::value_type< ResourceT > >
		ResourceManager::GetOrCreate(const key_type< ResourceT >& guid,
			                         ConstructorArgsT&&... args) {

		if constexpr (std::is_same_v< ModelDescriptor, ResourceT >) {
			return GetPool< ResourceT >().template 
				GetOrCreate< ID3D11Device&, ResourceManager&, key_type< ResourceT >, ConstructorArgsT... >(
					guid, m_device, *this, key_type< ResourceT >(guid), std::forward< ConstructorArgsT >(args)...);
		}
		else {
			return GetPool< ResourceT >().template 
				GetOrCreate< ID3D11Device&, key_type< ResourceT >, ConstructorArgsT... >(
					guid, m_device, key_type< ResourceT >(guid), std::forward< ConstructorArgsT >(args)...);
		}
	}

	#pragma endregion
}