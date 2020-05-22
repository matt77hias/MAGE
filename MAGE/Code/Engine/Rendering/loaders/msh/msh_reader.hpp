#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "io\binary_reader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <vector>

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations
//-----------------------------------------------------------------------------
namespace mage::rendering::loader {

	/**
	 A class of MSH file readers for reading meshes.

	 @tparam		VertexT
					The vertex type.
	 @tparam		IndexT
					The index type.
	 */
	template< typename VertexT, typename IndexT >
	class MSHReader : private BigEndianBinaryReader {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a MSH reader.

		 @param[in,out]	vertices
						A reference to a vector for storing the read vertices
						from file.
		 @param[in,out]	indices
						A reference to a vector for storing the read indices
						from file.
		 */
		explicit MSHReader(std::vector< VertexT >& vertices,
			               std::vector< IndexT >& indices);

		/**
		 Constructs a MSH reader from the given MSH reader.

		 @param[in]		reader
						A reference to the MSH reader to copy.
		 */
		MSHReader(const MSHReader& reader) = delete;

		/**
		 Constructs a MSH reader by moving the given MSH reader.

		 @param[in]		reader
						A reference to the MSH reader to move.
		 */
		MSHReader(MSHReader&& reader) noexcept;

		/**
		 Destructs this MSH reader.
		 */
		~MSHReader();

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given MSH reader to this MSH reader.

		 @param[in]		reader
						A reference to a MSH reader to copy.
		 @return		A reference to the copy of the given MSH reader (i.e.
						this MSH reader).
		 */
		MSHReader& operator=(const MSHReader& reader) = delete;

		/**
		 Moves the given MSH reader to this MSH reader.

		 @param[in]		reader
						A reference to a MSH reader to move.
		 @return		A reference to the moved MSH reader (i.e. this MSH
						reader).
		 */
		MSHReader& operator=(MSHReader&& reader) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		using BigEndianBinaryReader::ReadFromFile;

		using BigEndianBinaryReader::ReadFromMemory;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Starts reading.

		 @throws		Exception
						Failed to read from the given file.
		 */
		virtual void ReadData() override;

		/**
		 Checks whether the header of the file is valid.

		 @return		@c true if the header of the file is valid. @c false
						otherwise.
		 */
		[[nodiscard]]
		bool IsHeaderValid();

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 A reference to a vector containing the read vertices of this MSH
		 reader.
		 */
		std::vector< VertexT >& m_vertices;

		/**
		 A reference to a vector containing the read indices of this MSH
		 reader.
		 */
		std::vector< IndexT >& m_indices;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\msh\msh_reader.tpp"

#pragma endregion