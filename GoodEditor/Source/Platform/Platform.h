#pragma once

#include <filesystem>

namespace GoodEditor {

	class Platform
	{
	public:
		static void OpenDefaultApp(const std::filesystem::path& path);

		static std::filesystem::path GetPathFromDialog();
		static std::filesystem::path GetDirectoryFromDialog();
	};

}