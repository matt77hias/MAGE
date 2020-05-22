//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "io\binary_reader.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	void ReadBinaryFile(const std::filesystem::path& path,
						UniquePtr< U8[] >& data,
						std::size_t& size) {

		const auto file_handle
			= CreateUniqueHandle(CreateFile2(path.c_str(),
											 GENERIC_READ,
											 FILE_SHARE_READ,
											 OPEN_EXISTING,
											 nullptr));

		FILE_STANDARD_INFO file_info;
		{
			const BOOL result = GetFileInformationByHandleEx(file_handle.get(),
															 FileStandardInfo,
															 &file_info,
															 sizeof(file_info));
			ThrowIfFailed(result,
						  "{}: could not retrieve file information.", path);
			ThrowIfFailed((0 == file_info.EndOfFile.HighPart),
						  "{}: file too big for 32-bit allocation.", path);
		}

		// Allocate buffer.
		const auto nb_bytes = file_info.EndOfFile.LowPart;
		size = static_cast< std::size_t >(nb_bytes);
		data = MakeUnique< U8[] >(nb_bytes);
		ThrowIfFailed((nullptr != data),
					  "{}: file too big for allocation.", path);

		// Populate buffer.
		{
			DWORD nb_bytes_read = 0u;
			const BOOL result = ReadFile(file_handle.get(), data.get(),
										 nb_bytes, &nb_bytes_read, nullptr);
			ThrowIfFailed(result, "{}: could not load file data.", path);
			ThrowIfFailed((nb_bytes <= nb_bytes_read),
						  "{}: could not load all file data.", path);
		}
	}

	//-------------------------------------------------------------------------
	// BinaryReader
	//-------------------------------------------------------------------------
	#pragma region

	BinaryReader::BinaryReader()
		: m_path(),
		m_big_endian(true),
		m_pos(nullptr),
		m_end(nullptr),
		m_data() {}

	BinaryReader::BinaryReader(BinaryReader&& reader) noexcept = default;

	BinaryReader::~BinaryReader() = default;

	BinaryReader& BinaryReader
		::operator=(BinaryReader&& reader) noexcept = default;

	void BinaryReader
		::ReadFromFile(const std::filesystem::path& path, bool big_endian) {

		m_path       = path;
		m_big_endian = big_endian;

		std::size_t nb_bytes = 0u;
		ReadBinaryFile(m_path, m_data, nb_bytes);

		m_pos = m_data.get();
		m_end = m_data.get() + nb_bytes;

		ReadData();
	}

	void BinaryReader
		::ReadFromMemory(gsl::span< const U8 > input, bool big_endian) {

		m_path       = L"input string";
		m_big_endian = big_endian;

		m_pos = input.data();
		m_end = input.data() + input.size();

		ThrowIfFailed((m_pos <= m_end), "{}: overflow.", GetPath());

		ReadData();
	}

	NotNull< const_zstring > BinaryReader::ReadChars(std::size_t size) {
		const auto old_pos = m_pos;
		const auto new_pos = m_pos + size;
		ThrowIfFailed((m_pos <= new_pos),
					  "{}: overflow: no chars value found.", GetPath());
		ThrowIfFailed((new_pos <= m_end),
					  "{}: end of file: no chars value found.", GetPath());

		m_pos = new_pos;
		return NotNull< const_zstring >(reinterpret_cast< const char* >(old_pos));
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// BigEndianBinaryReader
	//-------------------------------------------------------------------------
	#pragma region

	BigEndianBinaryReader::BigEndianBinaryReader()
		: m_path(),
		m_pos(nullptr),
		m_end(nullptr),
		m_data() {}

	BigEndianBinaryReader::BigEndianBinaryReader(
		BigEndianBinaryReader&& reader) noexcept = default;

	BigEndianBinaryReader::~BigEndianBinaryReader() = default;

	BigEndianBinaryReader& BigEndianBinaryReader
		::operator=(BigEndianBinaryReader&& reader) noexcept = default;

	void BigEndianBinaryReader
		::ReadFromFile(const std::filesystem::path& path) {
		
		m_path = path;

		std::size_t nb_bytes = 0u;
		ReadBinaryFile(m_path, m_data, nb_bytes);

		m_pos  = m_data.get();
		m_end  = m_data.get() + nb_bytes;

		ReadData();
	}

	void BigEndianBinaryReader::ReadFromMemory(gsl::span< const U8 > input) {
		m_path = L"input string";

		m_pos  = input.data();
		m_end  = input.data() + input.size();

		ThrowIfFailed((m_pos <= m_end), "{}: overflow.", GetPath());

		ReadData();
	}

	#pragma endregion
}