#include "AssetPanel.h"

#include <Assets/Script.h>

#include "Platform/Platform.h"

namespace GoodEditor {

    const AssetPanel::ExtensionAssetLoadMap AssetPanel::s_ExtensionFunctions 
    { 
        {".png", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Image>(path); }},
        {".jpg", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Image>(path); } },
        {".tga", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Image>(path); } },
        {".bmp", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Image>(path); } },
        {".gif", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Image>(path); } },
        {".fbx", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Model>(path); } },
        {".obj", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Model>(path); } },
        {".hdr", [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<EnvironmentMap>(path); }},
        {".py",  [](const std::filesystem::path& path) { return  Project::GetSelectedProject()->GetAssetManager().LoadAssetIntoBank<Script>(path); }}
    };

	void AssetPanel::DrawPanel(std::map<std::filesystem::path, Ref<Image>>& extensionMap)
	{
        if (!ImGui::Begin("Asset Panel"))
        {
            ImGui::End();
            return;
        }

       auto selctedProject = Project::GetSelectedProject();

       auto& root = selctedProject->GetAssetDirectory();

       //if the current directory has not currently been set then set it to the root.
       if (s_CurrentDirectory.empty())
       {
           s_CurrentDirectory = root;

           if (!std::filesystem::exists(s_CurrentDirectory))
               std::filesystem::create_directories(s_CurrentDirectory);
       }
       
       //if user deletes the root automatically recreate it.

       if (!std::filesystem::exists(root))
       {
           s_CurrentDirectory = root;
           std::filesystem::create_directories(root);
       }

       uint32_t currentX = 0;

       const int buttonWidth = 120;
       const int buttonHeight = 100;

       //if current directory is not the root we want to be able to displaya back button.
       if (s_CurrentDirectory != root)
       {
           if (ImGui::ImageButton((ImTextureID)(intptr_t)extensionMap["back"]->GetTexture().GetHandle(), ImVec2(buttonWidth, buttonHeight), ImVec2(0, 1), ImVec2(1, 0)))
           {
               s_CurrentDirectory = s_CurrentDirectory.parent_path();
           }
           else
           {
               currentX += buttonWidth;
               ImGui::SameLine();
           }
       }

       ImVec2 size = ImGui::GetWindowSize();

       //if the user has deleted the current directory then it will keep going back until it reaches a valid one.
       while (!std::filesystem::exists(s_CurrentDirectory))
           s_CurrentDirectory = s_CurrentDirectory.parent_path();

       int k = 10000;

       //going through each file in the currently selected folder
       for (const auto& entry : std::filesystem::directory_iterator(s_CurrentDirectory))
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
                                                 ImVec2(buttonWidth, buttonHeight), ImVec2(1, 0), ImVec2(0, 1));

               ImGui::Text(filename.c_str());

               ImGui::EndGroup();

               if (pressed)
                   s_CurrentDirectory = entry.path();

               currentX += buttonWidth;

               if (currentX < size.x - buttonWidth)
                   ImGui::SameLine();
               else
                   currentX = 0;
               
               ImGui::PopID();
               continue;
           }

           //any extension not supported by the engine will be ignored
           if (!extensionMap.contains(extension))
           {
               ImGui::PopID();
               continue;
           }

          

            ImGui::BeginGroup();

            ImGui::ImageButton((ImTextureID)(intptr_t)extensionMap[extension]->GetTexture().GetHandle(), 
                                ImVec2(buttonWidth, buttonHeight));

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                Platform::OpenDefaultApp(path);

            //if we haven't loaded the assset in then do so and add it to the loaded directories
            if (!s_CachedDirectories.contains(path))
            {
                Ref<Asset> asset = s_ExtensionFunctions.at(extension)(path);

                //if the asset is valid and is a model we want to register this with the renderer
                if (asset && asset->GetAssetType() == AssetType::Model)
                {
                    Ref<Model> model = std::dynamic_pointer_cast<Model>(asset);

                    if (model)
                    {
                        ModelInfo info = selctedProject->GetCurrentScene()->GetSceneRenderer()->AddModel(model);
                        model->SetInfo(info);
                    }
                }

                if(asset)
                    s_CachedDirectories.insert(path);
            }
            
            //if the user begins to drag a file register it and add it to an imgui dragdrop payload.
            if (ImGui::BeginDragDropSource())
            {
                Ref<Asset> asset = selctedProject->GetAssetManager().FetchAsset(path);

                //if the asset isn't valid then try reload it.
                if (!asset)
                {
                    selctedProject->GetAssetManager().RemoveAsset(path);

                    asset = s_ExtensionFunctions.at(extension)(path);

                    if (asset && asset->GetAssetType() == AssetType::Model)
                    {
                        Ref<Model> model = std::dynamic_pointer_cast<Model>(asset);

                        if (model)
                        {
                            ModelInfo info = selctedProject->GetCurrentScene()->GetSceneRenderer()->AddModel(model);
                            model->SetInfo(info);

                            asset = model;
                        }
                    }
                }

                if (asset)
                {
                    TooGoodEngine::UUID id = asset->GetAssetID();

                    if (extension == ".png")
                        ImGui::SetDragDropPayload("IMAGE_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));
                    else if (extension == ".hdr")
                        ImGui::SetDragDropPayload("ENVIRONMENT_MAP_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));
                    else if (extension == ".fbx" || extension == ".obj")
                        ImGui::SetDragDropPayload("MODEL_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));
                    else
                        ImGui::SetDragDropPayload("SCRIPT_TRANSFER_UUID", &id, sizeof(TooGoodEngine::UUID));
                }
                

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

           ImGui::PopID();
       }


       //go through the cached directories to check their existance. If they do not exist we 
       //remove them from the asset manager.

       for (const auto& directory : s_CachedDirectories)
       {
           if (!std::filesystem::exists(directory))
           {
               selctedProject->GetAssetManager().RemoveAsset(directory);
               s_CachedDirectories.erase(directory);
           }
       }
        
       ImGui::End();
	}

}