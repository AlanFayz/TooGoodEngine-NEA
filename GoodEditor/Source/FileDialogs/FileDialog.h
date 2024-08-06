#pragma once

#include <Windows.h>
#include <filesystem>

namespace GoodEditor {
	class FileDialog
	{
	public:
		static std::filesystem::path GetPathFromDialog();
	};
}