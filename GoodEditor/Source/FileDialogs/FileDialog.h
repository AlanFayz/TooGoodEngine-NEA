#pragma once

#include <Windows.h>
#include <filesystem>

namespace GoodEditor {
	class FileDialog
	{
	public:
		static std::filesystem::path GetPathFromDialog();
		static std::filesystem::path GetDirectoryFromDialog();
	};
}