#include "Platform.h"

#include <Windows.h>     

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <shellapi.h>
#include <shobjidl_core.h> 
#include <comdef.h>        


namespace GoodEditor {

	void Platform::OpenDefaultApp(const std::filesystem::path& path)
	{
		std::string strPath = path.string();
		const char* cPath = strPath.c_str();

		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

		ShellExecuteA(glfwGetWin32Window(glfwGetCurrentContext()), "open", cPath, nullptr, nullptr, 0);;

		CoUninitialize();
	}

	std::filesystem::path Platform::GetPathFromDialog()
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

	std::filesystem::path Platform::GetDirectoryFromDialog()
	{
		HRESULT result;
		std::filesystem::path path;

		CoInitialize(NULL);

		IFileDialog* pfd = nullptr;
		CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

		DWORD dwOptions;
		pfd->GetOptions(&dwOptions);
		pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

		pfd->Show(glfwGetWin32Window(glfwGetCurrentContext()));

		IShellItem* pItem = nullptr;
		pfd->GetResult(&pItem);

		if (pItem)
		{
			PWSTR pszFolderPath = nullptr;
			result = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

			if (SUCCEEDED(result))
			{
				path = std::filesystem::path(pszFolderPath);
				CoTaskMemFree(pszFolderPath);
			}

			pItem->Release();
		}


		pfd->Release();

		CoUninitialize();
		return path;
	}
}