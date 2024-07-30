#include "FileReader.h"

namespace TooGoodEngine {
	
	FileReader::FileReader(const std::filesystem::path& path)
	{
		m_File.open(path);

		if (!m_File.is_open())
			TGE_LOG_WARNING("failed to open file ", path);
	}

	FileReader::~FileReader()
	{
		if(m_File.is_open())
			m_File.close();
	}

	StrongRef<MemoryAllocator> FileReader::Read(size_t bytes)
	{
		if (!m_File.is_open())
			return nullptr;
		
		StrongRef<MemoryAllocator> allocator = CreateStrongRef<MemoryAllocator>(bytes);

		char* converted = (char*)allocator->GetRaw();
		m_File.read(converted, bytes);

		return allocator;
	}

	void FileReader::SeekPosition(size_t position)
	{
		m_File.seekg(position);
	}

	const size_t FileReader::GetSize()
	{
		_VerifyStream();

		size_t current = GetPosition();

		m_File.seekg(0, std::ios_base::end);
		size_t off = GetPosition();

		m_File.seekg(current);

		return off;
	}

	void FileReader::_VerifyStream()
	{
		TGE_VERIFY(m_File.is_open(), "invalid stream");
	}

}