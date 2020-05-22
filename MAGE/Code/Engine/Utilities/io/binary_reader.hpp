#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "exception\exception.hpp"
#include "io\binary_utils.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Reads the bytes of the binary file associated with the given path.

	 @param[in]		path
					A reference to the path.
	 @param[in,out]	data
					A reference to a pointer to a buffer for storing the read
					bytes.
	 @param[in,out]	size
					A reference to the size of the read bytes.
	 @throws		Exception
					Failed to read from the file.
	 */
	void ReadBinaryFile(const std::filesystem::path& path,
						UniquePtr< U8[] >& data, std::size_t& size);

	//-------------------------------------------------------------------------
	// BinaryReader
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of readers for reading binary files.
	 */
	class BinaryReader {

	public:

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given binary reader to this binary reader.

		 @param[in]		reader
						A reference to a binary reader to copy.
		 @return		A reference to the copy of the given binary reader (i.e.
						this binary reader).
		 */
		BinaryReader& operator=(const BinaryReader& reader) = delete;

		/**
		 Moves the given binary reader to this binary reader.

		 @param[in]		reader
						A reference to a binary reader to move.
		 @return		A reference to the moved binary reader (i.e. this
						binary reader).
		 */
		BinaryReader& operator=(BinaryReader&& reader) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Reads from the given file associated with the given path.

		 @param[in]		path
						A reference to the path.
		 @param[in]		big_endian
						Flag indicating whether the given byte array should be
						interpreted as big endian or not (i.e. little endian).
		 @throws		Exception
						Failed to read from the file.
		 */
		void ReadFromFile(const std::filesystem::path& path, bool big_endian);

		/**
		 Reads the input string.

		 @param[in]		input
						The input byte string.
		 @param[in]		big_endian
						Flag indicating whether the given byte array should be
						interpreted as big endian or not (i.e. little endian).
		 @throws		Exception
						Failed to read from the given input string.
		 */
		void ReadFromMemory(gsl::span< const U8 > input, bool big_endian);

	protected:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a binary reader.
		 */
		BinaryReader();

		/**
		 Constructs a binary reader from the given binary reader.

		 @param[in]		reader
						A reference to the binary reader to copy.
		 */
		BinaryReader(const BinaryReader& reader) = delete;

		/**
		 Constructs a binary reader by moving the given binary reader.

		 @param[in]		reader
						A reference to the binary reader to move.
		 */
		BinaryReader(BinaryReader&& reader) noexcept;

		/**
		 Destructs this binary reader.
		 */
		~BinaryReader();

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the current path of this binary reader.

		 @return		A reference to the current path of this binary
						reader.
		 */
		[[nodiscard]]
		const std::filesystem::path& GetPath() const noexcept {
			return m_path;
		}

		/**
		 Checks if there are characters left to read by this binary reader.

		 @return		@c true if there are characters left to read by this
						binary reader. @c false otherwise.
		 */
		[[nodiscard]]
		bool ContainsChars() const noexcept {
			return m_pos < m_end;
		}

		/**
		 Reads an array of byte characters.

		 @param			size
						The number of bytes to read.
		 @return		A pointer to the array of characters read.
		 @throws		Exception
						Failed to read @c size bytes.
		 */
		NotNull< const_zstring > ReadChars(std::size_t size);

		/**
		 Reads a @c T value.

		 @tparam		T
						The data type.
		 @return		The @c T value read.
		 @throws		Exception
						Failed to read a @c T value.
		 */
		template< typename T >
		const T Read();

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Starts reading.

		 @throws		Exception
						Failed to read from the given file.
		 */
		virtual void ReadData() = 0;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The current path of this binary reader.
		 */
		std::filesystem::path m_path;

		/**
		 A flag indicating whether the current data of this binary reader
		 should be interpreted as big endian or not (i.e. little endian).
		 */
		bool m_big_endian;

		/**
		 A pointer to the current position of this binary reader.
		 */
		const U8* m_pos;

		/**
		 A pointer to the end position of this binary reader.
		 */
		const U8* m_end;

		/**
		 A pointer to the data to read of this binary reader.
		 */
		UniquePtr< U8[] > m_data;
	};

	#pragma endregion

	//-------------------------------------------------------------------------
	// BigEndianBinaryReader
	//-------------------------------------------------------------------------
	#pragma region

	/**
	 A class of readers for reading (big endian) binary files.
	 */
	class BigEndianBinaryReader {

	public:

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given big endian binary reader to this big endian binary
		 reader.

		 @param[in]		reader
						A reference to a big endian binary reader to copy.
		 @return		A reference to the copy of the given big endian binary
						reader (i.e. this big endian binary reader).
		 */
		BigEndianBinaryReader& operator=(
			const BigEndianBinaryReader& reader) = delete;

		/**
		 Moves the given big endian binary reader to this big endian binary
		 reader.

		 @param[in]		reader
						A reference to a big endian binary reader to move.
		 @return		A reference to the moved big endian binary reader
						(i.e. this big endian binary reader).
		 */
		BigEndianBinaryReader& operator=(
			BigEndianBinaryReader&& reader) noexcept;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Reads from the file associated with the given path.

		 @param[in]		path
						A reference to the path.
		 @throws		Exception
						Failed to read from the file.
		 */
		void ReadFromFile(const std::filesystem::path& path);

		/**
		 Reads the input string.

		 @param[in]		input
						The input byte string.
		 @throws		Exception
						Failed to read from the given input string.
		 */
		void ReadFromMemory(gsl::span< const U8 > input);

	protected:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a big endian binary reader.
		 */
		BigEndianBinaryReader();

		/**
		 Constructs a big endian binary reader from the given big endian binary
		 reader.

		 @param[in]		reader
						A reference to the big endian binary reader to copy.
		 */
		BigEndianBinaryReader(const BigEndianBinaryReader& reader) = delete;

		/**
		 Constructs a big endian binary reader by moving the given big endian
		 binary reader.

		 @param[in]		reader
						A reference to the big endian binary reader to move.
		 */
		BigEndianBinaryReader(BigEndianBinaryReader&& reader) noexcept;

		/**
		 Destructs this big endian binary reader.
		 */
		~BigEndianBinaryReader();

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Returns the current path of this big endian binary reader.

		 @return		A reference to the current path of this big endian
						binary reader.
		 */
		[[nodiscard]]
		const std::filesystem::path& GetPath() const noexcept {
			return m_path;
		}

		/**
		 Checks if there are characters left to read by this big endian binary
		 reader.

		 @return		@c true if there are characters left to read by this
						binary reader. @c false otherwise.
		 */
		[[nodiscard]]
		bool ContainsChars() const noexcept {
			return m_pos < m_end;
		}

		/**
		 Reads a @c T value.

		 @tparam		T
						The data type.
		 @return		The @c T value read.
		 @throws		Exception
						Failed to read a @c T value.
		 */
		template< typename T >
		const T Read();

		/**
		 Reads an array of @c T values.

		 @tparam		T
						The data type.
		 @param			count
						The number of @c T values to read.
		 @return		A pointer to the array of @c T value read.
		 @throws		Exception
						Failed to read @c count @c T values.
		 */
		template< typename T >
		const T* ReadArray(std::size_t count);

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Starts reading.

		 @throws		Exception
						Failed to read from the given file.
		 */
		virtual void ReadData() = 0;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The current path of this big endian binary reader.
		 */
		std::filesystem::path m_path;

		/**
		 A pointer to the current position of this big endian binary reader.
		 */
		const U8* m_pos;

		/**
		 A pointer to the end position of this big endian binary reader.
		 */
		const U8* m_end;

		/**
		 A pointer to the data to read of this big endian binary reader.
		 */
		UniquePtr< U8[] > m_data;
	};

	#pragma endregion
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "io\binary_reader.tpp"

#pragma endregion