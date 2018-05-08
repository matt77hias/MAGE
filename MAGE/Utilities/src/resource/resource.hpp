#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "type\types.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <filesystem>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 A class of resources.

	 @tparam		ResourceT
					The reource type.
	 */
	template< typename ResourceT >
	class Resource {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a resource.

		 @param[in]		guid
						The globally unique identifier.
		 */
		explicit Resource(wstring guid) noexcept;

		/**
		 Constructs a resource from the given resource.

		 @param[in]		resource
						A reference to the resource to copy.
		 */
		Resource(const Resource& resource) = delete;

		/**
		 Constructs a resource by moving the given resource.

		 @param[in]		resource
						A reference to the resource to move.
		 */
		Resource(Resource&& resource) noexcept;

		/**
		 Destructs this resource.
		 */
		virtual ~Resource();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given resource to this resource.

		 @param[in]		resource
						A reference to the resource to copy.
		 @return		A reference to the copy of the given resource (i.e.
						this resource).
		 */
		Resource& operator=(const Resource& resource) = delete;

		/**
		 Moves the given resource to this resource.

		 @param[in]		resource
						A reference to the resource to move.
		 @return		A reference to the moved resource (i.e. this resource).
		 */
		Resource& operator=(Resource&& resource) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the globally unique identifier of this resource.

		 @return		A reference to the globally unique identifier of this
						resource.
		 */
		[[nodiscard]]
		const wstring& GetGuid() const noexcept {
			return m_guid;
		}

		/**
		 Checks whether this resource represents a file resource.

		 @return		@c true if this resource represents a file resource.
						@c false otherwise.
		 */
		[[nodiscard]]
		bool IsFileResource() const;

		/**
		 Returns the path of this resource.

		 @return		The path of this resource.
		 @throws		Exception
						This resource does not represent a file resource.
		 */
		[[nodiscard]]
		const std::filesystem::path GetPath() const;

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The globally unique identifier of this resource.
		 */
		wstring m_guid;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "resource\resource.tpp"

#pragma endregion