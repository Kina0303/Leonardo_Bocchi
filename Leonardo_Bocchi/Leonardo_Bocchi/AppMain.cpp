#include "DxLib.h"
#include "Utility/InputControl.h"
#include "Objects/Player/Player.h"

//メインプログラム 開始

int WINAPI WinMain(_In_ HINSTANCE  hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR LpCmdLine, _In_ int NCmdShow) {

    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    SetMainWindowText("");         // ウィンドウタイトルを設定

    ChangeWindowMode(TRUE);       // ウインドウモードで起動

    //SetGraphMode(1280, 720, 32); // ウインドウのサイズ

    if (DxLib_Init() == -1) return -1;             // DXライブラリの初期化処理

    GameObject* objects[2] = { nullptr ,nullptr };  //ゲームオブジェクト初期化
    objects[0] = new GameObject();  //GameObjectを生成
    objects[1] = new Player();      //Playerオブジェクトを生成

    for (int i = 0; i < 2; i++)
    {
        objects[i]->Initialize();   //各オブジェクトの初期化処理
    }


    SetDrawScreen(DX_SCREEN_BACK);  // 描画先画面を裏にする（ダブルバッファリング）

    while (ProcessMessage() != 1) 
    {
        InputControl::Update();  //キー入力状態を更新

        //各オブジェクトの更新
        for (int i = 0; i < 2; i++)
        {
            objects[i]->Update();
        }

        ClearDrawScreen(); //画面をクリア

        //各オブジェクトの描画
        for (int i = 0; i < 2; i++)
        {
            objects[i]->Draw();
        }

        ScreenFlip(); // 裏画面の内容を表画面に反映する

        //終了
        if (InputControl::GetKeyUp(KEY_INPUT_ESCAPE))
        {
            break;
        }
    }

    DxLib_End(); // DXライブラリ使用の終了処理
    return 0;    // プログラムの終了
};