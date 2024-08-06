#include "FileDialog.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace GoodEditor {


	std::filesystem::path FileDialog::GetPathFromDialog()
	{
		char fileName[MAX_PATH]{};

		OPENFILENAMEA dialogBoxInfo{};
		dialogBoxInfo.lStructSize = sizeof(OPENFILENAMEA);
		dialogBoxInfo.hwndOwner = glfwGetWin32Window(glfwGetCurrentContext());
		dialogBoxInfo.lpstrFile = fileName;
		dialogBoxInfo.lpstrTitle = "Open Project";
		dialogBoxInfo.nMaxFile = MAX_PATH;
		dialogBoxInfo.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		BOOL fileOpened = GetOpenFileNameA(&dialogBoxInfo);
		
		if (fileOpened)
			return std::filesystem::path(fileName);

		return std::filesystem::path();
	}

}
