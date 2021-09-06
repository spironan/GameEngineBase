#include "RenderingTestLayer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"


oom::mat4 matrix{ 1.0f };

ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;

engine::ooRendererID fb;

void RenderingTestLayer::OnImGuiRender()
{
    fb = engine::FramebufferPool::Get("2D_Framebuffer")->GetImage()->GetID();

    m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->SetCamera(m_camera.GetComponent<engine::SceneCamera>(), m_camera.GetComponent<engine::Transform3D>().GetGlobalPosition());
    m_scene.GetWorld().GetSystem<engine::Renderer2DSystem>()->Update();

    auto& transform = m_child.GetComponent<engine::Transform3D>();

    engine::Window& x = engine::Application::Get().GetWindow();
    int width = x.GetSize().first;
    int height = x.GetSize().second;

    auto ar = (float)width / height;


    ImGui::Begin("Scene", nullptr,
                    ImGuiWindowFlags_NoBringToFrontOnFocus
                 | ImGuiWindowFlags_NoFocusOnAppearing
                 //| ImGuiWindowFlags_NoMove
                 //| ImGuiWindowFlags_NoMouseInputs
                 );

        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        vMin.x += ImGui::GetWindowPos().x;
        vMin.y += ImGui::GetWindowPos().y;
        vMax.x += ImGui::GetWindowPos().x;
        vMax.y += ImGui::GetWindowPos().y;

        ImVec2 vpDim = { vMax.x - vMin.x ,vMax.y - vMin.y };
        auto myW = vMax.x - vMin.x;
        auto myH = vMax.y - vMin.y;

        // opengl loves saving framebuffers upside down, 
        // so we flip the UVW in imgui::image()
        ImGui::Image((ImTextureID)static_cast<uint64_t>(fb), ImVec2{ ar*myH, myH }, { 0.0f,1.0f }, { 1.0f,0.0f });
        
        //Debug Red box
        //ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, ImU32(0xFF0000FF));

        ImGui::SetNextWindowSize({ vMax.x - vMin.x,vMax.y - vMin.y });
        ImGui::SetNextWindowPos(vMin);

        //window hole should be same size as content area
        ImGui::SetWindowHitTestHole(ImGui::GetCurrentWindow(), vMin, { myW,myH });

        // IMPORTANT: we now NEED to call this before begin frame
        ImGuizmo::SetRect(vMin.x, vMin.y, ar*myH, myH);
        ImGuizmo::BeginFrame(); 

        oom::vec3 mScale = transform.GetGlobalScale();
        oom::vec3 mPosition = transform.GetGlobalPosition();
        oom::vec3 mRot{0,0,(transform.GetGlobalRotationRad())};
        

        // Set the object TRS
        ImGuizmo::RecomposeMatrixFromComponents(oom::value_ptr(mPosition), oom::value_ptr(mRot), oom::value_ptr(mScale), oom::value_ptr(matrix));

        // Do your picking here. Then recompose matrix to update
        if (engine::Input::IsKeyPressed(engine::KeyCode::Q))
        {
            operation = ImGuizmo::TRANSLATE;
        } else if (engine::Input::IsKeyPressed(engine::KeyCode::W))
        {
            operation = ImGuizmo::ROTATE;
        } else if (engine::Input::IsKeyPressed(engine::KeyCode::E))
        {
            operation = ImGuizmo::SCALE;
        }

        auto& cam = m_camera.GetComponent<engine::SceneCamera>();
        auto& camTrans = m_camera.GetComponent<engine::Transform3D>();

        // TODO : way for camera to do view matrix
        auto& projection = cam.GetProjection();
        auto& view = cam.CalculateViewMatrix(camTrans.GetGlobalMatrix());

        auto mCurrentGizmoMode = ImGuizmo::WORLD;
        ImGuizmo::SetOrthographic(true);

        // supposed to call here but i am editing the interals to support docking
        //ImGuizmo::SetRect(vMin.x, vMin.y, myW, myH);
        ImGuizmo::SetDrawlist();
        if (ImGuizmo::Manipulate(oom::value_ptr(view),
                                 oom::value_ptr(projection),
                                 operation,
                                 mCurrentGizmoMode,
                                 oom::value_ptr(matrix),
                                 NULL,
                                 NULL))
        {
             // Return the matrix to its components so that we can update values
            ImGuizmo::DecomposeMatrixToComponents(oom::value_ptr(matrix),
                                                  oom::value_ptr(mPosition),
                                                  oom::value_ptr(mRot),
                                                  oom::value_ptr(mScale));
            transform.SetPosition(mPosition);
            transform.SetScale(mScale);
            transform.SetRotationAngle(oom::degrees(mRot.z));
        }

       
        // transform.SetRotationAxis(mRot); // maybe??

   
    ImGui::End(); // End of Scene
   

    //    if (ImGuizmo::IsUsing() == false)
    //    {
    //        LOG_ENGINE_INFO("{0} {1}", mScale.x, mScale.y);
    //}
        //ImGui::Begin("Hello");
        //auto x = engine::TextureDatabase::GetTexture("ogre");
        //ImGui::Image(reinterpret_cast<ImTextureID>(x.id), ImVec2{ (float)x.width,  (float)x.height });
        //ImGui::End();
    
}
