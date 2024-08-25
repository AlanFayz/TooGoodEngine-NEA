#pragma once

#include "Core/Base.h"
#include "Core/MemoryAllocator.h"

#include <filesystem>
#include <fstream>

namespace TooGoodEngine {

	class FileReader
	{
	public:
		FileReader(const std::filesystem::path& path, bool isBinary);
		~FileReader();

		inline const size_t GetPosition() { _VerifyStream(); return m_File.tellg(); };

		MemoryAllocator<char> Read(size_t bytes);
		void  SeekPosition(size_t position);
		const size_t GetSize();

	private:
		void _VerifyStream();

	private:
		std::ifstream m_File;
	};

}