#include "Core/EntryPoint.h"
#include "Editor/Editor.h"

using namespace TooGoodEngine;

int main()
{
	ApplicationData data{};
	data.Layers.push_back(CreateRef<GoodEditor::Editor>());
	data.Title        = "Editor";
	data.WindowWidth  = 1200;
	data.WindowHeight = 800;

	return TooGoodEngine::EngineMain(data);
}