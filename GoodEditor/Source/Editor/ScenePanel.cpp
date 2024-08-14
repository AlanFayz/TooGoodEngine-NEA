#include "ScenePanel.h"

#include "ECS/Components/Components.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace GoodEditor {

	int ScenePanel::m_IDCount = 0;
	int ScenePanel::m_ColorEditFlags = ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs;

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

		m_IDCount = 1000;
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

		if (tree.HasComponent<PointLightComponent>(entity))
			_DrawComponent(tree.GetComponent<PointLightComponent>(entity));

		if (tree.HasComponent<DirectionalLightComponent>(entity))
			_DrawComponent(tree.GetComponent<DirectionalLightComponent>(entity));
	}
	void ScenePanel::_DrawSettings(const Ref<Scene>& scene)
	{
		RenderSettings renderSettings = scene->GetSceneRenderer()->GetSettings();

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

		ImGui::SeparatorText("Tone mapping");

		if (ImGui::DragFloat("Gradient", &renderSettings.Gradient, 0.01f, 0.0f, FLT_MAX / 2.0f))
			changed = true;

		ImGui::SeparatorText("Enviorment map");

		if (ImGui::DragFloat("Level of detail", &renderSettings.LevelOfDetail, 0.01f, 0.0f, (float)g_NumberOfMipMaps - 1.0f))
			changed = true;

		if (renderSettings.CurrentEnviormentMap)
		{
			uint32_t handle = renderSettings.CurrentEnviormentMap->GetTexture().GetHandle();
			if (ImGui::Button("Remove"))
			{
				renderSettings.CurrentEnviormentMap.reset();
				renderSettings.CurrentEnviormentMap = nullptr;
				changed = true;
			}

			ImGui::SameLine();
			ImGui::Text("Enviorment Map");
		}
		else
		{

			float col[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImGui::ColorEdit4("Enviorment Map", col, ImGuiColorEditFlags_AlphaPreview |
				ImGuiColorEditFlags_NoInputs |
				ImGuiColorEditFlags_NoPicker);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENVIORMENT_MAP_TRANSFER_UUID"))
				{
					UUID id = *(UUID*)payload->Data;
					renderSettings.CurrentEnviormentMap = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<EnviormentMap>(id);
				}

				changed = true;
				ImGui::EndDragDropTarget();
			}
		}


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

			if (_DrawMaterialAttribute("Ambient", component.Material.Ambient.Component, component.Material.Ambient.ImageComponent))
				changed = true;

			if (_DrawMaterialAttribute("Albedo", component.Material.Albedo.Component, component.Material.Albedo.ImageComponent))
				changed = true;

			if (_DrawMaterialAttribute("Metallic", component.Material.Metallic.Component, component.Material.Metallic.ImageComponent))
				changed = true;

			if (_DrawMaterialAttribute("Emission", component.Material.Emission.Component, component.Material.Emission.ImageComponent))
				changed = true;

			if (_DrawMaterialRoughness(component.Material.Roughness.Component.x, component.Material.Roughness.ImageComponent))
				changed = true;

			if (ImGui::DragFloat("Albedo Factor", &component.Material.AlbedoFactor, 0.01f, 0.0f, FLT_MAX / 2))
				changed = true;

			if (ImGui::DragFloat("Metallic Factor", &component.Material.MetallicFactor, 0.01f, 0.0f, FLT_MAX / 2))
				changed = true;

			if (ImGui::DragFloat("Emission Factor", &component.Material.EmissionFactor, 0.1f, 0.0f, FLT_MAX / 2))
				changed = true;

			if (changed)
			{
				component.Material.UpdateTypes();
				g_SelectedProject->GetCurrentScene()->GetSceneRenderer()->ChangeMaterialData(component.ID, component.Material);
			}

			ImGui::TreePop();
		}
	}
	void ScenePanel::_DrawComponent(PointLightComponent& component)
	{
		if (ImGui::TreeNode("Point Light"))
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color), m_ColorEditFlags);
			ImGui::DragFloat3("Position", glm::value_ptr(component.Position), 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::DragFloat("Radius", &component.Radius, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::TreePop();
		}
	}
	void ScenePanel::_DrawComponent(DirectionalLightComponent& component)
	{
		if (ImGui::TreeNode("Directional Light"))
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color), m_ColorEditFlags);
			ImGui::DragFloat3("Direction", glm::value_ptr(component.Direction), 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::TreePop();
		}
	}
	bool ScenePanel::_DrawMaterialAttribute(const char* name, glm::vec4& attribute, Ref<Image>& image)
	{
		bool changed = false;

		std::string imageName = name;
		imageName += " Image";

		if (ImGui::ColorEdit4(name, glm::value_ptr(attribute), m_ColorEditFlags))
			changed = true;

		ImGui::PushID(m_IDCount++);
		if (image)
		{
			uint32_t handle = image->GetTexture().GetHandle();
			if (ImGui::ImageButton((ImTextureID)(intptr_t)handle, ImVec2(15, 15), ImVec2(0, 1), ImVec2(1, 0)))
			{
				image.reset();
				image = nullptr;
				changed = true;
			}

			ImGui::SameLine();
			ImGui::Text(imageName.c_str());
		}
		else
		{
			
			float col[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImGui::ColorEdit4(imageName.c_str(), col, ImGuiColorEditFlags_AlphaPreview | 
													  ImGuiColorEditFlags_NoInputs | 
													  ImGuiColorEditFlags_NoPicker);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				image = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(id);
			}

			changed = true;
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		return changed;
	}
	bool ScenePanel::_DrawMaterialRoughness(float& roughness, Ref<Image>& image)
	{
		bool changed = false;

		ImGui::PushID(m_IDCount++);

		if (image)
		{
			uint32_t handle = image->GetTexture().GetHandle();
			if (ImGui::ImageButton((ImTextureID)(intptr_t)handle, ImVec2(15, 15), ImVec2(0, 1), ImVec2(1, 0)))
			{
				image.reset();
				image = nullptr;
				changed = true;
			}
			ImGui::SameLine();
			ImGui::Text("Roughness");
		}
		else
		{
			float col[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImGui::ColorEdit4("Roughness Image", col, m_ColorEditFlags |
													  ImGuiColorEditFlags_NoPicker);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				image = g_SelectedProject->GetAssetManager().FetchAssetAssuredType<Image>(id);
			}

			changed = true;
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		if (ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.0f, "%.3f"))
			changed = true;

		return changed;
	}
}