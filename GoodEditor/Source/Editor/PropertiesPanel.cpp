#include "PropertiesPanel.h"

#include "Assets/Script.h"
#include "Scripting/ScriptingEngine.h"

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GoodEditor {
	
	int PropertiesPanel::s_ColorEditFlags = ImGuiColorEditFlags_NoInputs;

	void PropertiesPanel::DrawPanel()
	{
		if (!ImGui::Begin("Properties"))
		{
			ImGui::End();
			return;
		}

		auto selectedProject = Project::GetSelectedProject();
		Ref<Scene> currentScene = selectedProject->GetCurrentScene();
		auto& tree = currentScene->GetRegistry();

		EntityID entity = ScenePanel::GetSelectedEntity();
		
		//if the entity is not valid we simply return
		if (entity == g_NullEntity)
		{
			ImGui::End();
			return;
		}

		const std::string& name = tree.GetEntityByID(entity).GetName();

		ImGui::Text(name.c_str());

		//draw all possible components if the entity contains them.
		if (tree.HasComponent<TransformComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<TransformComponent>(entity));
			if (_DeleteComponentPopup("Transform Component"))
				tree.RemoveComponent<TransformComponent>(entity);

		}

		if (tree.HasComponent<MeshComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<MeshComponent>(entity));
			if (_DeleteComponentPopup("Mesh Component"))
				tree.RemoveComponent<MeshComponent>(entity);
		}

		if (tree.HasComponent<MaterialComponent>(entity))
		{
			auto& component = tree.GetComponent<MaterialComponent>(entity);
			_DrawComponent(component);
			if (_DeleteComponentPopup("Material Component"))
			{
				component.Renderer->RemoveMaterial(component.ID);
				tree.RemoveComponent<MaterialComponent>(entity);
			}
		}

		if (tree.HasComponent<PointLightComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<PointLightComponent>(entity));
			if (_DeleteComponentPopup("Point Light Component"))
				tree.RemoveComponent<PointLightComponent>(entity);
		}

		if (tree.HasComponent<DirectionalLightComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<DirectionalLightComponent>(entity));
			if (_DeleteComponentPopup("Directional Light Component"))
				tree.RemoveComponent<DirectionalLightComponent>(entity);
		}
		
		if (tree.HasComponent<ModelComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<ModelComponent>(entity));
			if (_DeleteComponentPopup("Model Component"))
				tree.RemoveComponent<ModelComponent>(entity);
		}

		if (tree.HasComponent<ScriptComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<ScriptComponent>(entity));
			if (_DeleteComponentPopup("Script Component"))
				tree.RemoveComponent<ScriptComponent>(entity);
		}

		if (tree.HasComponent<OrthographicCameraComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<OrthographicCameraComponent>(entity));
			if (_DeleteComponentPopup("Orthographic Camera Component"))
				tree.RemoveComponent<OrthographicCameraComponent>(entity);
		}

		if (tree.HasComponent<PerspectiveCameraComponent>(entity))
		{
			_DrawComponent(tree.GetComponent<PerspectiveCameraComponent>(entity));
			if (_DeleteComponentPopup("Perspective Camera Component"))
				tree.RemoveComponent<PerspectiveCameraComponent>(entity);
		}

	
		ImGui::End();

		s_IDCount = 100000;
	}

	void PropertiesPanel::_DrawComponent(TransformComponent& component)
	{
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::DragFloat3("Position", glm::value_ptr(component.Position), 0.1f, -FLT_MAX / 2, FLT_MAX / 2);
			ImGui::DragFloat3("Rotation", glm::value_ptr(component.Rotation), 0.1f, -FLT_MAX / 2, FLT_MAX / 2);
			ImGui::DragFloat3("Scale",	  glm::value_ptr(component.Scale),    0.1f, -FLT_MAX / 2, FLT_MAX / 2);

			ImGui::TreePop();
		}

	}
	void PropertiesPanel::_DrawComponent(MeshComponent& component)
	{
		if (ImGui::TreeNode("Mesh"))
		{
			ImGui::Text("Mesh ID: %llu", component.ID);
			ImGui::Text(component.PathToSource.c_str());
			ImGui::TreePop();
		}

	}
	void PropertiesPanel::_DrawComponent(MaterialComponent& component)
	{
		if (ImGui::TreeNode("Material"))
		{
			auto& materialInfo = component.Renderer->GetMaterialInfo(component.ID);

			bool changed = false;

			if (_DrawMaterialAttribute("Ambient", materialInfo.Ambient, materialInfo.AmbientTexture))
				changed = true;

			if (_DrawMaterialAttribute("Albedo", materialInfo.Albedo, materialInfo.AlbedoTexture))
				changed = true;

			if (_DrawMaterialAttribute("Emission", materialInfo.Emission, materialInfo.EmissionTexture))
				changed = true;

			if (_DrawMaterialSingle("Roughness", materialInfo.Roughness, materialInfo.RoughnessTexture))
				changed = true;

			if (_DrawMaterialSingle("Metalness", materialInfo.Metallic, materialInfo.MetallicTexture))
				changed = true;

			if (ImGui::DragFloat("Emission Factor", &materialInfo.EmissionFactor, 0.1f, 0.0f, FLT_MAX / 2))
				changed = true;

			if (changed)
			{
				component.Renderer->ModifyMaterial(component.ID, materialInfo);
			}

			ImGui::TreePop();
		}
	}
	void PropertiesPanel::_DrawComponent(PointLightComponent& component)
	{
		if (ImGui::TreeNode("Point Light"))
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color), s_ColorEditFlags);
			ImGui::DragFloat3("Position", glm::value_ptr(component.Position), 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::DragFloat("Radius", &component.Radius, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::TreePop();
		}
	}
	void PropertiesPanel::_DrawComponent(DirectionalLightComponent& component)
	{
		if (ImGui::TreeNode("Directional Light"))
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color), s_ColorEditFlags);
			ImGui::DragFloat3("Direction", glm::value_ptr(component.Direction), 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f);
			ImGui::TreePop();
		}
	}
	void PropertiesPanel::_DrawComponent(ModelComponent& component)
	{
		ImGui::PushID(s_IDCount++);


		if (ImGui::TreeNode("Model"))
		{
			auto selectedProject = Project::GetSelectedProject();


			Ref<Asset> asset = selectedProject->GetAssetManager().FetchAsset(component.ModelAssetId);

			if (asset)
			{
				std::filesystem::path path = asset->GetPath();
				std::string pathString = path.string();

				ImGui::Text("Path: %s", pathString.c_str());
			}

			ImGui::TreePop();
		}

		//if the user has dropped a model asset onto the component then create the component using that model.
		if (ImGui::BeginDragDropTarget())
		{
			auto selectedProject = Project::GetSelectedProject();

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				Ref<Model> asset = selectedProject->GetAssetManager().FetchAssetAssuredType<Model>(id);

				component.ModelAssetId = asset->GetAssetID();
				component.Info = asset->GetInfo();
			}

			ImGui::EndDragDropTarget();
		}


		ImGui::PopID();
	}
	void PropertiesPanel::_DrawComponent(ScriptComponent& component)
	{
		ImGui::PushID(s_IDCount++);

		if (ImGui::TreeNode("Script"))
		{
			auto selectedProject = Project::GetSelectedProject();

			Ref<Asset> asset = selectedProject->GetAssetManager().FetchAsset(component.GetHandle());

			if (asset)
			{
				std::filesystem::path path = asset->GetPath();
				std::string pathString = path.string();

				ImGui::Text("Path: %s", pathString.c_str());
			}

			if (asset)
			{
				//if the user clicks the refersh button then it will destroy the script component and re extract it 
				if (ImGui::Button("Refresh"))
				{
					component.~ScriptComponent();

					ScriptData data = ScriptingEngine::ExtractScript(asset->GetPath());

					if (data.PyOnCreate && data.PyOnUpdate)
						component = ScriptComponent(data);
					else
						component = ScriptComponent();

					component.SetHandle(asset->GetAssetID());
				}

			}


			ImGui::TreePop();
		}

		//if the user drops a script asset onto the component then create the script from that asset.

		if (ImGui::BeginDragDropTarget())
		{
			auto selectedProject = Project::GetSelectedProject();


			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				Ref<Script> asset = selectedProject->GetAssetManager().FetchAssetAssuredType<Script>(id);

				ScriptData data = ScriptingEngine::ExtractScript(asset->GetPath());

				if (data.PyOnCreate && data.PyOnUpdate)
				{
					component.Create(data);
					component.SetHandle(id);
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}
	void PropertiesPanel::_DrawComponent(PerspectiveCameraComponent& component)
	{
		ImGui::PushID(s_IDCount++);

		if (ImGui::TreeNode("Perspective Camera"))
		{
			bool changed = false;

			if (ImGui::DragFloat("Fov", &component.data.Fov, 0.1f, 1.0f, 180.0f))
				changed = true;

			if (ImGui::DragFloat("Aspect Ratio", &component.data.AspectRatio, 0.1f, 0.001f, 10.0f))
				changed = true;

			if (ImGui::DragFloat("Near", &component.data.Near, 0.01f, 0.01f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat("Far", &component.data.Far, 0.01f, 0.01f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Position", glm::value_ptr(component.data.Position), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Front", glm::value_ptr(component.data.Front), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Up", glm::value_ptr(component.data.Up), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			ImGui::Checkbox("Use", &component.InUse);

			if (changed)
				component.Camera->SetData(component.data);

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	void PropertiesPanel::_DrawComponent(OrthographicCameraComponent& component)
	{
		ImGui::PushID(s_IDCount++);

		if (ImGui::TreeNode("Orthographic Camera"))
		{
			bool changed = false;

			if (ImGui::DragFloat("Left", &component.data.Left, 0.01f, -FLT_MAX / 2.0, FLT_MAX / 2.0))
				changed = true;

			if (ImGui::DragFloat("Right", &component.data.Right, 0.01f, -FLT_MAX / 2.0, FLT_MAX / 2.0))
				changed = true;

			if (ImGui::DragFloat("Top", &component.data.Top, 0.01f, -FLT_MAX / 2.0, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat("Bottom", &component.data.Bottom, 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Position", glm::value_ptr(component.data.Position), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Front", glm::value_ptr(component.data.Front), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;

			if (ImGui::DragFloat3("Up", glm::value_ptr(component.data.Up), 0.01f, -FLT_MAX / 2.0f, FLT_MAX / 2.0f))
				changed = true;


			ImGui::Checkbox("Use", &component.InUse);

			if (changed)
				component.Camera->SetData(component.data);

			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	bool PropertiesPanel::_DrawMaterialAttribute(const char* name, glm::vec4& attribute, Ref<Image>& image)
	{
		bool changed = false;

		std::string imageName = name;
		imageName += " Image";

		if (ImGui::ColorEdit3(name, glm::value_ptr(attribute), s_ColorEditFlags))
			changed = true;

		ImGui::PushID(s_IDCount++);
		if (image)
		{
			uint32_t handle = image->GetTexture().GetHandle();
			if (ImGui::ImageButton((ImTextureID)(intptr_t)handle, ImVec2(15, 15), ImVec2(1, 0), ImVec2(0, 1)))
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

		//similar to the rest of the drop targets
		if (ImGui::BeginDragDropTarget())
		{
			auto selectedProject = Project::GetSelectedProject();

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				image = selectedProject->GetAssetManager().FetchAssetAssuredType<Image>(id);
			}

			changed = true;
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		return changed;
	}
	bool PropertiesPanel::_DrawMaterialSingle(const char* name, float& value, Ref<Image>& image)
	{
		bool changed = false;

		ImGui::PushID(s_IDCount++);

		if (image)
		{
			uint32_t handle = image->GetTexture().GetHandle();
			if (ImGui::ImageButton((ImTextureID)(intptr_t)handle, ImVec2(15, 15), ImVec2(1, 0), ImVec2(0, 1)))
			{
				image.reset();
				image = nullptr;
				changed = true;
			}
			ImGui::SameLine();
			ImGui::Text(name);
		}
		else
		{
			std::string imgName = std::string(name) + " Image";
			float col[] = { 0.0f, 0.0f, 0.0f, 0.0f };
			ImGui::ColorEdit4(imgName.c_str(), col, s_ColorEditFlags |
				ImGuiColorEditFlags_NoPicker);
		}

		//similar to the rest of the drop targets
		if (ImGui::BeginDragDropTarget())
		{
			auto selectedProject = Project::GetSelectedProject();

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("IMAGE_TRANSFER_UUID"))
			{
				UUID id = *(UUID*)payload->Data;
				image = selectedProject->GetAssetManager().FetchAssetAssuredType<Image>(id);
			}

			changed = true;
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();

		if (ImGui::DragFloat(name, &value, 0.01f, 0.0f, 1.0f, "%.3f"))
			changed = true;

		return changed;
	}
	bool PropertiesPanel::_DeleteComponentPopup(const char* name)
	{
		bool shouldDelete = false;


		if (ImGui::BeginPopupContextItem(name))
		{
			if (ImGui::MenuItem("Delete"))
				shouldDelete = true;

			ImGui::EndPopup();
		}

		return shouldDelete;
	}
}