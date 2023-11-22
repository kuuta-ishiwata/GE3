
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "SpriteCommon.h"
#include "Sprite.h"

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

        input_->Update();
        

        //更新後処理
        dxCommon_->PreDraw();
        
        sprite_->Draw();

        //更新前処理
        dxCommon_->PostDraw();

        // DirectX毎フレーム処理　ここまで

    }

    delete sprite_;
    delete common;

    delete input_;
    delete dxCommon_;
    winApp_->Finalize();

    delete winApp_;
    
    return 0;
}
