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
                if (ImGui::ImageButton((void*)extensionMap["back"]->GetTexture().GetHandle(), ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0)))
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

            for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
            {
                std::filesystem::path path = entry.path();
                std::filesystem::path extension = path.extension();
                std::string filename = path.filename().string();
                std::string sPath = path.string();

                if (entry.is_directory())
                {
                    ImGui::BeginGroup();
                    bool pressed = ImGui::ImageButton((void*)extensionMap["folder"]->GetTexture().GetHandle(),
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
                        continue;

                    ImGui::BeginGroup();

                    ImGui::ImageButton((void*)extensionMap[path.extension()]->GetTexture().GetHandle(),
                        ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0));

                    ImGui::Text(filename.c_str());

                    ImGui::EndGroup();

                    if (!m_CachedDirectories.contains(path))
                    {
                        //TODO: audio asset when supported needs to be here as well
                        if (extension == ".png")
                            g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Image>(path);
                        if (extension == ".fbx" || extension == ".obj")
                            g_SelectedProject->GetAssetManager().LoadAssetIntoBank<Model>(path);

                        m_CachedDirectories.insert(path);
                    }

                    currentX += buttonWidth;

                    if (currentX < size.x - buttonWidth)
                        ImGui::SameLine();
                    else
                        currentX = 0;
                    
                }

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