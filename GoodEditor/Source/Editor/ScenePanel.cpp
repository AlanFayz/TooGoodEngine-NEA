#include "ScenePanel.h"

#include "ECS/Components/Components.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace GoodEditor {

	void ScenePanel::DrawScenePanel()
	{
		Ref<Scene> currentScene = g_SelectedProject->GetCurrentScene();
		auto& registry = currentScene->GetRegistry();

		std::unordered_set<EntityID> displayedEntities;
		
		ImGui::Begin("Scene Panel");

		if (ImGui::TreeNode("Scene Settings"))
		{
			_DrawSettings(currentScene);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Entities"))
		{
			for (EntityID i = 0; i < registry.GetCount(); i++)
			{
				Entity entity = registry.GetEntity(i);
				auto& node    = registry.GetNode(entity);

				//already displayed or going to be displayed from parent
				if (displayedEntities.contains(i) || node.ParentIndex != g_NullNode)
					continue;

				displayedEntities.insert(i); 

				if (ImGui::TreeNode(entity.GetName().c_str()))
				{
					_DrawEntity(entity, registry);
					_DrawChildren(entity, registry, displayedEntities); //recursively call

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

	
		ImGui::End();
	}
	void ScenePanel::_DrawChildren(const Entity& entity, EntityTree& tree, std::unordered_set<EntityID>& displayed)
	{
		auto& node = tree.GetNode(entity);

		for (size_t i = 0; i < node.Children.size(); i++)
		{
			if (displayed.contains(node.Children[i]))
				continue;

			Entity childEntity = tree.GetEntity(node.Children[i]);

			displayed.insert(node.Children[i]);

			if (ImGui::TreeNode(childEntity.GetName().c_str()))
			{
				_DrawEntity(childEntity, tree);
				_DrawChildren(childEntity, tree, displayed);

				ImGui::TreePop();
			}
		}
	}
	void ScenePanel::_DrawEntity(const Entity& entity, EntityTree& tree)
	{
		if (tree.HasComponent<TransformComponent>(entity))
			_DrawComponent(tree.GetComponent<TransformComponent>(entity));

		if (tree.HasComponent<MeshComponent>(entity))
			_DrawComponent(tree.GetComponent<MeshComponent>(entity));

		if (tree.HasComponent<MaterialComponent>(entity))
			_DrawComponent(tree.GetComponent<MaterialComponent>(entity));
	}

	void ScenePanel::_DrawSettings(const Ref<Scene>& scene)
	{
		auto renderSettings = scene->GetSceneRenderer()->GetSettings();

		bool changed = false;

		ImGui::SeparatorText("Clear Color");

		if (ImGui::ColorPicker4("##ClearColor", glm::value_ptr(renderSettings.ClearColor)))
			changed = true;

		ImGui::SeparatorText("Depth Testing");

		static int  s_DepthTest = (int)DepthTestOption::Less;
		const char* depthTestNames[] = { "None", "Less", "LessOrEqual", "Equal", "Greater" };
		const char* depthTestName = (s_DepthTest >= 0 && s_DepthTest < (int)DepthTestOption::Count) ? depthTestNames[s_DepthTest] : "Unknown";
		if (ImGui::SliderInt("##DepthTesting", &s_DepthTest, 0, (int)DepthTestOption::Count - 1, depthTestName))
			changed = true;

		renderSettings.DepthTesting = (DepthTestOption)s_DepthTest;

		ImGui::SeparatorText("Culling");

		static int  s_CullingOption = (int)CullingOption::None;
		const char* cullingOptionNames[] = { "None", "Back", "Front", "FrontAndBack"};
		const char* cullingOptionName = (s_CullingOption >= 0 && s_CullingOption < (int)CullingOption::Count) ? cullingOptionNames[s_CullingOption] : "Unknown";
		if (ImGui::SliderInt("##CullingOption", &s_CullingOption, 0, (int)CullingOption::Count - 1, cullingOptionName))
			changed = true;

		renderSettings.Culling = (CullingOption)s_CullingOption;

		ImGui::SeparatorText("Winding Order");

		static int  s_WindingOrder = (int)WindingOrderOption::CounterClockWise;
		const char* windingOrderNames[] = { "None", "ClockWise", "CounterClockWise" };
		const char* windingOrderName = (s_WindingOrder >= 0 && s_WindingOrder < (int)WindingOrderOption::Count) ? windingOrderNames[s_WindingOrder] : "Unknown";
		if (ImGui::SliderInt("##WindingOrder", &s_WindingOrder, 0, (int)WindingOrderOption::Count - 1, windingOrderName))
			changed = true;
		
		renderSettings.WindingOrder = (WindingOrderOption)s_WindingOrder;

		ImGui::SeparatorText("Blending Options");

		ImGui::Text("C = color, F = factor, s = source, d = dest");
		ImGui::Text("Equation: R = C(s) * F(s) + C(d) * F(d)");

		static int s_BlendFactorSource = (int)BlendingFactor::None;
		static int s_BlendFactorDst = (int)BlendingFactor::None;

		const char* blendingFactorNames[] = { "None", "Zero", "SrcColor", "DstColor", "OneMinusSrcColor",
		"OneMinusDstColor", "SrcAlpha", "OneMinusSrcAlpha", "DstAlpha", "OneMinusDstAlpha" };

		const char* blendSrcName = (s_BlendFactorSource >= 0 && s_BlendFactorSource < (int)BlendingFactor::Count) ? blendingFactorNames[s_BlendFactorSource] : "Unknown";
		const char* dstSrcName   = (s_BlendFactorDst >= 0    && s_BlendFactorDst < (int)BlendingFactor::Count) ? blendingFactorNames[s_BlendFactorDst] : "Unknown";

		if (ImGui::SliderInt("Source Blending Factor", &s_BlendFactorSource, 0, (int)BlendingFactor::Count - 1, blendSrcName))
			changed = true;

		if (ImGui::SliderInt("Destination Blending Factor", &s_BlendFactorDst, 0, (int)BlendingFactor::Count - 1, dstSrcName))
			changed = true;

		renderSettings.Source      = (BlendingFactor)s_BlendFactorSource;
		renderSettings.Destination = (BlendingFactor)s_BlendFactorDst;

		ImGui::SeparatorText("Rasterizer Mode");

		static int s_RasterMode = (int)FillMode::Fill;

		const char* rasterModeNames[] = { "None", "Point", "Line", "Fill"};
		const char* rasterModeName = (s_RasterMode >= 0 && s_RasterMode < (int)FillMode::Count) ? rasterModeNames[s_RasterMode] : "Unknown";

		if (ImGui::SliderInt("##RasterMode", &s_RasterMode, 0, (int)FillMode::Count - 1, rasterModeName))
			changed = true;

		renderSettings.FillingMode = (FillMode)s_RasterMode;

		if (changed)
			scene->GetSceneRenderer()->ChangeSettings(renderSettings);
	}
    void ScenePanel::_DrawComponent(TransformComponent& component)
	{
		auto& position = component.GetPosition();
		auto& scale = component.GetScale();
		auto& rotation = component.GetRotation();

		if (ImGui::TreeNode("Transform"))
		{
			bool changed = false;

			if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f, -FLT_MAX / 2, FLT_MAX / 2))
				changed = true;

			if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f, -FLT_MAX / 2, FLT_MAX / 2))
				changed = true;

			if (ImGui::DragFloat3("Scale",    glm::value_ptr(scale),    0.1f, -FLT_MAX / 2, FLT_MAX / 2))
				changed = true;

			if (changed)
				component.UpdateTransform();

			ImGui::TreePop();
		}

	}
	void ScenePanel::_DrawComponent(MeshComponent& component)
	{
		if (ImGui::TreeNode("Mesh"))
		{
			ImGui::Text("Mesh ID: %llu", component.ID);
			ImGui::Text(component.PathToSource.c_str());
			ImGui::TreePop();
		}
	
	}
	void ScenePanel::_DrawComponent(MaterialComponent& component)
	{
		if (ImGui::TreeNode("Material"))
		{
			bool changed = false;

			//TODO: add buttons to remove/add new images

			//
			// ---- Ambient ----
			if (component.Material.Ambient.ImageComponent)
			{
				ImGui::Image((void*)component.Material.Ambient.ImageComponent->GetTexture().GetHandle(), ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				if (ImGui::DragFloat4("Ambient", glm::value_ptr(component.Material.Ambient.Component), 0.01f, 0.0f, 1.0f))
					changed = true;
			}

			//
			// ---- Albedo ----
			if (component.Material.Albedo.ImageComponent)
			{
				ImGui::Image((void*)component.Material.Albedo.ImageComponent->GetTexture().GetHandle(), ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				if (ImGui::DragFloat4("Albedo", glm::value_ptr(component.Material.Albedo.Component), 0.01f, 0.0f, 1.0f))
					changed = true;
			}

			if (ImGui::DragFloat("Albedo Factor", &component.Material.AlbedoFactor, 0.01f, 0.0f, FLT_MAX / 2))
				changed = true;

			//
			// ---- Metallic ----
			if (component.Material.Metallic.ImageComponent)
			{
				ImGui::Image((void*)component.Material.Metallic.ImageComponent->GetTexture().GetHandle(), ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				if (ImGui::DragFloat4("Metallic", glm::value_ptr(component.Material.Metallic.Component), 0.01f, 0.0f, 1.0f))
					changed = true;
			}

			if (ImGui::DragFloat("Metallic Factor", &component.Material.MetallicFactor, 0.01f, 0.0f, FLT_MAX / 2))
				changed = true;

			//
			// ---- Emission ----
			if (component.Material.Emission.ImageComponent)
			{
				ImGui::Image((void*)component.Material.Emission.ImageComponent->GetTexture().GetHandle(), ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				if (ImGui::DragFloat4("Emission", glm::value_ptr(component.Material.Emission.Component), 0.01f,  0.0f, 1.0f))
					changed = true;
			}

			if (ImGui::DragFloat("Emission Factor", &component.Material.EmissionFactor, 0.1f, 0.0f, FLT_MAX / 2))
				changed = true;

			//
			// ---- Roughness ----
			if (component.Material.Roughness.ImageComponent)
			{
				ImGui::Image((void*)component.Material.Roughness.ImageComponent->GetTexture().GetHandle(), ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0));
			}
			else
			{
				if (ImGui::DragFloat("Roughness", glm::value_ptr(component.Material.Roughness.Component), 0.01f, 0.0f, 1.0f))
					changed = true;
			}

			if (changed)
				g_SelectedProject->GetCurrentScene()->GetSceneRenderer()->ChangeMaterialData(component.ID, component.Material);

			ImGui::TreePop();
		}
	}
}