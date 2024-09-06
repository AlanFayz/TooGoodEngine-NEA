#include "Core/EntryPoint.h"
#include "Editor/Editor.h"

int main()
{
	//entry point into the engine
	TooGoodEngine::ApplicationData data{};
	data.Layers.push_back(TooGoodEngine::CreateRef<GoodEditor::Editor>());
	data.Title        = "Good Editor";
	data.WindowWidth  = 1200;
	data.WindowHeight = 800;

	return TooGoodEngine::EngineMain(data);
}