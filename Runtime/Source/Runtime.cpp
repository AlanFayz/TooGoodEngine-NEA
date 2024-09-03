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
		m_SelectedProject = TooGoodEngine::Project::CreateProject(m_ProjectPath);
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
	m_SelectedProject->GetCurrentScene()->Play(delta);
	m_SelectedProject->GetCurrentScene()->GetSceneRenderer()->RenderImageToScreen(m_CurrentWidth, m_CurrentHeight);
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
		m_SelectedProject->GetCurrentScene()->OnEvent(&viewportResize);
	}
}
