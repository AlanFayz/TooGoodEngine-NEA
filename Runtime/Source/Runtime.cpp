#include "Runtime.h"

#include <Windows.h>

Runtime::Runtime(const std::filesystem::path& path)
	: m_ProjectPath(path)
{
}

void Runtime::OnCreate()
{
	try
	{
		TooGoodEngine::g_SelectedProject = TooGoodEngine::CreateRef<TooGoodEngine::Project>(m_ProjectPath);
		TooGoodEngine::g_SelectedProject->LoadProject();
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "failed to create the project", 0);
		exit(-3);
	}
}

void Runtime::OnDestroy()
{
}

void Runtime::OnUpdate(double delta)
{
	TooGoodEngine::g_SelectedProject->GetCurrentScene()->Play(delta);
	TooGoodEngine::g_SelectedProject->GetCurrentScene()->GetSceneRenderer()->RenderImageToScreen(m_CurrentWidth, m_CurrentHeight);
}

void Runtime::OnGuiUpdate(double delta)
{
}

void Runtime::OnEvent(TooGoodEngine::Event* event)
{
	if (event->GetType() == TooGoodEngine::EventType::WindowResize)
	{
		TooGoodEngine::WindowResizeEvent* windowResize = (TooGoodEngine::WindowResizeEvent*)event;
		m_CurrentWidth  = windowResize->GetWidth();
		m_CurrentHeight = windowResize->GetHeight();


		TooGoodEngine::ViewportResizeEvent viewportResize(m_CurrentWidth, m_CurrentHeight);
		TooGoodEngine::g_SelectedProject->GetCurrentScene()->OnEvent(&viewportResize);
	}
}
