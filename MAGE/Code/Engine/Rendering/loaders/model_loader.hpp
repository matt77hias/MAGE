#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "resource\model\model_output.hpp"
#include "resource\mesh\mesh_descriptor.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage::rendering {
	// Forward declaration.
	class ResourceManager;
}

namespace mage::rendering::loader {

	/**
	 Imports the model from the file associated with the given path.

	 @tparam		VertexT
					The vertex type.
	 @tparam		IndexT
					The index type.
	 @param[in]		path
					A reference to the path.
	 @param[in,out]	resource_manager
					A reference to the resource manager.
	 @param[in,out]	model_output
					A reference to the model output.
	 @param[in]		mesh_desc
					A reference to the mesh descriptor.
	 @throws		Exception
					Failed to import the model from file.
	 */
	template< typename VertexT, typename IndexT >
	void ImportModelFromFile(const std::filesystem::path& path,
							 ResourceManager& resource_manager,
							 ModelOutput< VertexT, IndexT >& model_output,
							 const MeshDescriptor< VertexT, IndexT >&
							 mesh_desc = MeshDescriptor< VertexT, IndexT >());

	/**
	 Exports the model to the file associated with the given path.

	 @tparam		VertexT
					The vertex type.
	 @tparam		IndexT
					The index type.
	 @param[in]		path
					A reference to the path.
	 @param[in]		model_output
					A reference to the model output.
	 @throws		Exception
					Failed to export the model to file.
	 */
	template< typename VertexT, typename IndexT >
	void ExportModelToFile(const std::filesystem::path& path,
						   const ModelOutput< VertexT, IndexT >& model_output);
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\model_loader.tpp"

#pragma endregion