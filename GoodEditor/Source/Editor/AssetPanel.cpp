#include "AssetPanel.h"

#include "FileDialogs/FileDialog.h"

namespace GoodEditor {


	void AssetPanel::DrawAssets()
	{
        ImGui::Begin("Asset Panel");

        if (ImGui::TreeNode("Assets"))
        {
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
                ImGui::OpenPopup("AssetPannelPopUp");

            _ShowAssetPannelPopup();

            int i = 0;
            for (const auto& [key, asset] : g_SelectedProject->GetAssetManager().GetBank())
            {
                if (i == 0)
                    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                //gonna be unique anyways may as well use it lol
                ImGui::PushID((uint64_t)key);
                if (ImGui::TreeNode("", "Asset %d", i))
                {
                    std::string filePathStr    = asset->GetPath().string();
                    std::string_view assetName = GetAssetTypeString(asset->GetAssetType());

                    std::string assetIdLabel = "Asset UUID: %" + std::string(PRIu64);

                    ImGui::Text("Asset Type: %s", assetName.data());
                    ImGui::Text("Asset File Path: %s", filePathStr.c_str());
                    ImGui::Text(assetIdLabel.c_str(), (uint64_t)key);
                    ImGui::TreePop();
                }
                ImGui::PopID();

                i++;
            }
            ImGui::TreePop();
        }

        ImGui::End();
	}

    void AssetPanel::_ShowAssetPannelPopup()
    {
        if (ImGui::BeginPopupContextItem("AssetPannelPopUp"))
        {
            if (ImGui::MenuItem("Add Image Asset"))
            {
                std::filesystem::path path = FileDialog::GetPathFromDialog();

                if (std::filesystem::exists(path) && path.extension() == ".png")
                    g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);

            }

            if (ImGui::MenuItem("Add Model Asset"))
            {
                std::filesystem::path path = FileDialog::GetPathFromDialog();

                if (std::filesystem::exists(path) && (path.extension() == ".fbx" || path.extension() == ".obj"))
                    g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Model>(path);
            }

            ImGui::EndPopup();
        }
    }

}