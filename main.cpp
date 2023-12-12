
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ImGuiManager.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

   //ポインタ置き場
    Input* input_ = nullptr;
    WinApp* winApp_ = nullptr;
    DirectXCommon* dxCommon_ = nullptr;
#pragma region WindowsAPI初期化処理

    winApp_ = new WinApp();
    winApp_->Initialize();

    
#pragma endregion

#pragma region 

    dxCommon_ = new DirectXCommon();
    dxCommon_->Initialize(winApp_);


    // DirectX初期化処理　ここまで
#pragma endregion

    input_ = new Input();
    input_->Initialize(winApp_);
    
    
    ImGuiManager* imgui = new ImGuiManager();
    ImGuiManager::Initialize(winApp_->GetHwnd(), dxCommon_);

    SpriteCommon* common = new SpriteCommon();
    common->Initialize(dxCommon_);

    Sprite* sprite_ = new Sprite();
    sprite_->Initialize(dxCommon_, common);


    // ゲームループ
    while (true) {
        
        //更新
        if (winApp_->Update() == true)
        {
            break;
        }

        ImGuiManager::NewFrame();
        imgui->ShowDemo();

        input_->Update();
        

        //更新後処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();
        
        sprite_->Draw();

        //更新前処理
        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }

    delete sprite_;
    delete common;

    delete imgui;

    delete input_;
    delete dxCommon_;
    winApp_->Finalize();

    delete winApp_;
    
    return 0;
}
