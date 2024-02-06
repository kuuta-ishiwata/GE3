
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ImGuiManager.h"

#include <vector>
#include "TextureManager.h"

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

    TextureManager::GetInstance()->Initialize(dxCommon_);
    TextureManager::GetInstance()->LoadTexture(L"Resources/mario.jpg");
    TextureManager::GetInstance()->LoadTexture(L"Resources/reimu.png");

    std::vector<Sprite*> sprite_;
    for(int i = 0; i< 5;i++)
    {
        Sprite* temp = new Sprite();
        if(i%2 ==0)         temp->Initialize(common,L"Resources/mario.jpg");
        else if (i % 2 == 1)temp->Initialize(common, L"Resources/reimu.png");
        temp->SetPosition({ (float)i * 120,0 });
        sprite_.push_back(temp);

    }

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

        //移動
        /*
        DirectX::XMFLOAT2 pos = sprite_->GetPosition();
        pos.x += 0.01f;
        sprite_->SetPosition(pos);


        //回転

        float rot = sprite_->GetRotation();
        rot += 0.01f;
        sprite_->SetRotation(rot);

        //色
        DirectX::XMFLOAT4 color = sprite_->GetColor();
        color.x -= 0.1f;
        if (color.x < 0)
        {

            color.x = 1.0f;
        }
        sprite_->SetColor(color);


        //サイズ
        DirectX::XMFLOAT2 size = sprite_->Getize();
        size.y += 0.01f;
        sprite_->SetSize(size);
        */

        for (int i = 0; i < 5; i++)
        {
            sprite_[i]->Update();
        }

        

        //更新後処理
        ImGuiManager::CreateCommand();
        dxCommon_->PreDraw();
       
        for (int i = 0; i < 5; i++)
        {
            sprite_[i]->Draw();
        }
        //更新前処理
        ImGuiManager::CommandsExcute(dxCommon_->GetCommandList());
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }
    for (int i = 0; i < 5; i++)
    {
        delete sprite_[i];
    }

    TextureManager::GetInstance()->Finalize();

    delete common;

    delete imgui;

    delete input_;
    delete dxCommon_;
    winApp_->Finalize();

    delete winApp_;
    
    return 0;
}
