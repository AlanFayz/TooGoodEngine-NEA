#include "Runtime.h"

#include <Core/EntryPoint.h>
#include <Core/Files/Json.h>

#include <windows.h>
#include <string>

enum class ErrorCodes
{
	NonExistingProject = -1, InvalidProjectFile = -2, EmptyFilePath = -3
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (__argc < 1)
    {
        MessageBoxA(NULL, "Empty file path", "project doesn't exist", 0);
        return (int)(ErrorCodes::EmptyFilePath);
    }

    std::filesystem::path project(__argv[1]);

    if (!std::filesystem::exists(project))
    {
        MessageBoxA(NULL, __argv[1], "project doesn't exist", 0);
        return (int)(ErrorCodes::NonExistingProject);
    }

    TooGoodEngine::ApplicationData app;

    try
    {
        TooGoodEngine::JsonReader reader(project);

        app.Title = reader.Fetch<std::string>({ "Project Name" });
        app.WindowWidth = 1200;
        app.WindowHeight = 800;
        app.Layers.push_back(std::make_shared<Runtime>(project));
    }
    catch (const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), "invalid project file", 0);
        return (int)(ErrorCodes::InvalidProjectFile);
    }


    return TooGoodEngine::EngineMain(app);
}