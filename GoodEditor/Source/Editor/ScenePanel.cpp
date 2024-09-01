#include "ScenePanel.h"

#include <ECS/Components/Components.h>
#include <Assets/Script.h>
#include <Scripting/ScriptingEngine.h>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace GoodEditor {

	int ScenePanel::s_IDCount = 0;
	int ScenePanel::s_ColorEditFlags = ImGuiColorEditFlags_NoInputs;
	EntityID ScenePanel::s_CurrentEntity = g_NullEntity;
	EntityID ScenePanel::s_SelectedEntity = g_NullEntity;
	bool ScenePanel::s_EntityNamePopup = false;

	void ScenePanel::DrawPanel()
	{
		Ref<Scene> currentScene = g_SelectedProject->GetCurrentScene();
		auto& registry = currentScene->GetRegistry();
		auto renderer = currentScene->GetSceneRenderer();

		std::unordered_set<EntityID> displayedEntities;
		
		if (!ImGui::Begin("Scene Panel"))
		{
			ImGui::End();
			return;
		}

		if (ImGui::TreeNode("Scene Settings"))
		{
			_DrawSettings(currentScene);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Entities"))
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Add Entity"))
					registry.Add("Entity" + std::to_string(currentScene->GetRegistry().GetCount()));

				ImGui::EndPopup();
			}

			for (EntityID i = 0; i < registry.GetCount(); i++)
			{
				Entity& entity = registry.GetEntityByID(i);

				//deleted or null entity
				if (!entity)
					continue;

				auto& node = registry.GetNode(entity);

				//already displayed or going to be displayed from parent
				if (displayedEntities.contains(i) || node.ParentIndex != g_NullNode)
					continue;

				displayedEntities.insert(i);

				ImGui::PushID(s_IDCount++);

				if (ImGui::TreeNode(entity.GetName().c_str()))
				{
					if (!_EntityPopup(entity, renderer, registry)) //returns true if entity has been deleted
					{
						_DrawEntity(entity, registry, renderer);
						_DrawChildren(entity, registry, renderer, displayedEntities); //recursively call
					}

					ImGui::TreePop();
				}

				if (ImGui::IsItemClicked())
					s_SelectedEntity = entity;


				ImGui::PopID();
			}

			ImGui::TreePop();
		}


		if (s_CurrentEntity != g_NullEntity && ImGui::Begin("Entity Name", &s_EntityNamePopup))
		{
			static char buf[50];

			ImGui::InputText("##Name", buf, 50);

			if (ImGui::Button("Enter"))
			{
				std::string name = buf;
				Entity& entity = registry.GetEntityByID(s_CurrentEntity);
				if (name.length() > 0)
					entity.SetName(name);

				s_EntityNamePopup = false;
				s_CurrentEntity = g_NullEntity;
			}

			if (ImGui::Button("Close"))
			{
				s_EntityNamePopup = false;
				s_CurrentEntity = g_NullEntity;
			}

			ImGui::End();
		}
		else
		{
			s_CurrentEntity = g_NullEntity;
		}


		ImGui::End();
		s_IDCount = 1000;
	}

	EntityID ScenePanel::GetSelectedEntity()
	{
		return s_SelectedEntity;
	}

	void ScenePanel::_DrawChildren(Entity& entity, EntityTree& tree, const Ref<Renderer>& sceneRenderer, std::unordered_set<EntityID>& displayed)
	{
		if (!entity)
			return;

		auto& node = tree.GetNode(entity);

		for (size_t i = 0; i < node.Children.size(); i++)
		{
			if (displayed.contains(node.Children[i]))
				continue;

			Entity childEntity = tree.GetEntityByID(node.Children[i]);

			if (!childEntity)
				continue;

			displayed.insert(node.Children[i]);

			if (ImGui::TreeNode(childEntity.GetName().c_str()))
			{
				if (!_EntityPopup(childEntity, sceneRenderer, tree))
				{
					_DrawEntity(childEntity, tree, sceneRenderer);
					_DrawChildren(childEntity, tree, sceneRenderer, displayed);
				}

				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked())
				s_SelectedEntity = entity;
		}
	}
	void ScenePanel::_DrawEntity(Entity& entity, EntityTree& tree, const Ref<Renderer>& sceneRenderer)
	{
		if (ImGui::IsItemClicked())
			s_SelectedEntity = entity;

		if (ImGui::Button("Rename"))
		{
			s_CurrentEntity = entity.GetID();
			s_EntityNamePopup = true;
		}
	}

	bool ScenePanel::_EntityPopup(Entity& entity, const Ref<Renderer>& sceneRenderer, EntityTree& tree)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove"))
			{
				tree.RemoveEntity(entity);
				ImGui::EndPopup();
				return true;
			}

			if (ImGui::MenuItem("Add Quad") && !tree.HasComponent<MeshComponent>(entity))
			{
				MeshComponent component{};
				component.ID = 0;
				component.PathToSource = "##Quad";
				tree.AddComponent(entity, component);
			}

			if (ImGui::MenuItem("Add Cube") && !tree.HasComponent<MeshComponent>(entity))
			{
				MeshComponent component{};
				component.ID = 1;
				component.PathToSource = "##Cube";
				tree.AddComponent(entity, component);
			}

			if (ImGui::MenuItem("Add Transform") && !tree.HasComponent<TransformComponent>(entity))
				tree.EmplaceComponent<TransformComponent>(entity);

			if (ImGui::MenuItem("Add Material") && !tree.HasComponent<MaterialComponent>(entity))
			{
				MaterialComponent component{};
				component.ID = sceneRenderer->CreateMaterial();
				component.Renderer = sceneRenderer;

				tree.AddComponent(entity, component);
			}

			if (ImGui::MenuItem("Add Point Light") && !tree.HasComponent<PointLightComponent>(entity))
				tree.EmplaceComponent<PointLightComponent>(entity);

			if (ImGui::MenuItem("Add Directional Light") && !tree.HasComponent<DirectionalLightComponent>(entity))
				tree.EmplaceComponent<DirectionalLightComponent>(entity);

			if (ImGui::MenuItem("Add Model") && !tree.HasComponent<ModelComponent>(entity))
				tree.EmplaceComponent<ModelComponent>(entity);

			if (ImGui::MenuItem("Add Script") && !tree.HasComponent<ScriptComponent>(entity))
				tree.EmplaceComponent<ScriptComponent>(entity);

			if (ImGui::MenuItem("Add Perspective Camera") && !tree.HasComponent<PerspectiveCameraComponent>(entity))
			{
				PerspectiveCameraComponent component;
				component.Camera = CreateRef<PerspectiveCamera>(component.data);

				tree.AddComponent(entity, component);
			}

			if (ImGui::MenuItem("Add Orthographic Camera") && !tree.HasComponent<OrthographicCameraComponent>(entity))
			{
				OrthographicCameraComponent component;
				component.Camera = CreateRef<OrthographicCamera>(component.data);

				tree.AddComponent(entity, component);
			}


			if (ImGui::MenuItem("Add Entity"))
				tree.Add(entity, "Entity" + std::to_string(tree.GetCount()));

			ImGui::EndPopup();
		}

		return false;
	}
	void ScenePanel::_DrawSettings(const Ref<Scene>& scene)
	{
		RenderSettings renderSettings = scene->GetSceneRenderer()->GetSettings();

		bool changed = false;

		ImGui::SeparatorText("Clear Color");

		if (ImGui::ColorEdit4("##ClearColor", glm::value_ptr(renderSettings.ClearColor), ImGuiColorEditFlags_AlphaPreview |ImGuiColorEditFlags_NoInputs))
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

		ImGui::SeparatorText("Tone mapping");

		if (ImGui::DragFloat("Gradient", &renderSettings.Gradient, 0.01f, 0.0f, FLT_MAX / 2.0f))
			changed = true;

		ImGui::SeparatorText("Environment map");

		if (ImGui::DragFloat("Level of detail", &renderSettings.LevelOfDetail, 0.01f, 0.0f, (float)g_NumberOfMipMaps - 1.0f))
			changed = true;

		if (renderSettings.CurrentEnvironmentMap)
		{
			uint32_t handle = renderSettings.CurrentEnvironmentMap->GetTexture().GetHandle();
			if (ImGui::Button("Remove"))
			{
				renderSettings.CurrentEnvironmentMap.reset();
				renderSettings.CurrentEnvironmentMap = nullptr;
				changed = true;
			}

			ImGui::SameLine();
			ImGui::Text("Environment Map");
		}
		else
		{

			float col[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImGui::ColorEdit4("Environment Map", col, ImGuiColorEditFlags_AlphaPreview |
				ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_NoPicker);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENVIRONMENT_MAP_TRANSFER_UUID"))
				{
					UUID id = *(UUID*)payload->Data;
					renderSettings.CurrentEnvironmentMap = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<EnvironmentMap>(id);
				}

				changed = true;
				ImGui::EndDragDropTarget();
			}
		}

		ImGui::SeparatorText("Bloom Settings");

		if (ImGui::Checkbox("Enable Bloom", &renderSettings.Bloom))
			changed = true;

		if (ImGui::DragFloat("Threshold", &renderSettings.Threshold, 0.01f, 0.0f, FLT_MAX / 2.0f))
			changed = true;

		if (ImGui::DragFloat("Intensity", &renderSettings.Intensity, 0.01f, 0.0f, FLT_MAX / 2.0f))
			changed = true;

		if (ImGui::DragFloat("Filter Radius", &renderSettings.FilterRadius, 0.001f, 0.0f, 1.0f))
			changed = true;

		if (changed)
			scene->GetSceneRenderer()->ChangeSettings(renderSettings);
	}
 
}