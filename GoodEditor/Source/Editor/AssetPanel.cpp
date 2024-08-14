#include "AssetPanel.h"

#include "FileDialogs/FileDialog.h"

namespace GoodEditor {

    std::set<std::filesystem::path> AssetPanel::m_CachedDirectories = {};
    std::filesystem::path           AssetPanel::m_CurrentDirectory = "";

	void AssetPanel::DrawAssetPanel(std::map<std::filesystem::path, Ref<Image>>& extensionMap)
	{
        if (ImGui::Begin("Asset Panel"))
        {
            if (m_CurrentDirectory.empty())
            {
                m_CurrentDirectory = g_SelectedProject->GetAssetDirectory();

                if (!std::filesystem::exists(m_CurrentDirectory))
                    std::filesystem::create_directories(m_CurrentDirectory);
            }

            uint32_t currentX = 0;

            const int buttonWidth = 120;
            const int buttonHeight = 100;

            if (m_CurrentDirectory != g_SelectedProject->GetAssetDirectory())
            {
                if (ImGui::ImageButton((ImTextureID)(intptr_t)extensionMap["back"]->GetTexture().GetHandle(), ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0)))
                {
                    m_CurrentDirectory = m_CurrentDirectory.parent_path();
                }
                else
                {
                    currentX += buttonWidth;
                    ImGui::SameLine();
                }
            }

            ImVec2 size = ImGui::GetWindowSize();

            while (!std::filesystem::exists(m_CurrentDirectory))
                m_CurrentDirectory = m_CurrentDirectory.parent_path();

            int k = 10000;
            for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
            {
                std::filesystem::path path = entry.path();
                std::filesystem::path extension = path.extension();
                std::string filename = path.filename().string();
                std::string sPath = path.string();

                ImGui::PushID(k++);

                if (entry.is_directory())
                {
                    ImGui::BeginGroup();
                    bool pressed = ImGui::ImageButton((ImTextureID)(intptr_t)extensionMap["folder"]->GetTexture().GetHandle(),
                        ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0));

                    ImGui::Text(filename.c_str());

                    ImGui::EndGroup();

                    if (pressed)
                        m_CurrentDirectory = entry.path();

                    currentX += buttonWidth;

                    if (currentX < size.x - buttonWidth)
                        ImGui::SameLine();
                    else
                        currentX = 0;
                    
                }
                else
                {
                    //any extension not supported by the engine will be ignored
                    if (!extensionMap.contains(extension))
                    {
                        ImGui::PopID();
                        continue;
                    }

                    ImGui::BeginGroup();

                    ImGui::ImageButton((ImTextureID)(intptr_t)extensionMap[path.extension()]->GetTexture().GetHandle(),
                        ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0));

                    if (!m_CachedDirectories.contains(path))
                    {
                        //TODO: audio asset when supported needs to be here as well
                        if (extension == ".png")
                            g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
                        if (extension == ".fbx" || extension == ".obj")
                            g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Model>(path);
                        if (extension == ".hdr")
                            g_SelectedProject->GetAssetManager().LoadAssetIntoBank<EnvironmentMap>(path);

                        m_CachedDirectories.insert(path);
                    }

                    if (ImGui::BeginDragDropSource())
                    {
                        Ref<Asset> asset = g_SelectedProject->GetAssetManager().FetchAsset(path);

                        if (!asset)
                        {
                            g_SelectedProject->GetAssetManager().RemoveAsset(path);
                            if (extension == ".png")
                                asset = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
                            else
                                asset = g_SelectedProject->GetAssetManager().LoadAssetIntoBank<EnvironmentMap>(path);
                        }

                        TooGoodEngine::UUID id = asset->GetAssetID();

                        if(extension == ".png")
                            ImGui::SetDragDropPayload("IMAGE_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));
                        else 
                            ImGui::SetDragDropPayload("Environment_MAP_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));


                        ImGui::Text(filename.c_str());
                        ImGui::EndDragDropSource();
                    }

                    ImGui::Text(filename.c_str());

                    ImGui::EndGroup();

                    currentX += buttonWidth;

                    if (currentX < size.x - buttonWidth)
                        ImGui::SameLine();
                    else
                        currentX = 0;
                    
                }

                ImGui::PopID();
            }

            ImGui::End();

            for (const auto& directory : m_CachedDirectories)
            {
                //check if file has been deleted
                if (!std::filesystem::exists(directory))
                {
                    g_SelectedProject->GetAssetManager().RemoveAsset(directory);
                    m_CachedDirectories.erase(directory);
                }
            }
        }   
	}

}