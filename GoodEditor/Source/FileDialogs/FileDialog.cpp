#include "FileDialog.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <shobjidl_core.h> 
#include <comdef.h>        

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

	std::filesystem::path FileDialog::GetDirectoryFromDialog()
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
