#pragma once

#include <Core/Base.h>
#include <Project/Project.h>
#include <Core/Layers/Layer.h>

class Runtime : public TooGoodEngine::Layer
{
public:
	Runtime(const std::filesystem::path& path);
	~Runtime() = default;

	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(double delta) override;
	virtual void OnGuiUpdate(double delta) override;
	virtual void OnEvent(TooGoodEngine::Event* event) override;

private:
	std::filesystem::path m_ProjectPath;

	TooGoodEngine::Ref<TooGoodEngine::Project> m_SelectedProject; //guarenteed to be one for the entire runtime

	uint32_t m_CurrentWidth = 1200;
	uint32_t m_CurrentHeight = 800;
};