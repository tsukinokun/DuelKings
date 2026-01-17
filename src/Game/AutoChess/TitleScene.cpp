//---------------------------------------------------------------------------
//!	@file	TitleScene.cpp
//! @brief	オートチェスのタイトルシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "TitleScene.h"
#include <Game/AutoChess/InGameScene.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Funiture/Glass.h>
#include <system/Component/ComponentModel.h>
#include <Game/AutoChess/UIObject/UIButton.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <System/Component/ComponentCamera.h>
//---------------------------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------------------------
bool TitleScene::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------
    // カメラを生成
    //---------------------------------------------------------------------------
    auto camera = Scene::Object::Create<Object>();
    //アングルを変える
    auto com_comp = camera->AddComponent<ComponentCamera>();
    com_comp->SetPositionAndTarget({0.0f, 2.0f, -8.0f}, {0.0f, 2.0f, 0.0f});
    //---------------------------------------------------------------------------------
    // 机の生成
    //---------------------------------------------------------------------------------
    {
        auto table = Scene::Object::Create<Glass>();
    }
    //---------------------------------------------------------------------------------
    // 部屋の生成
    //---------------------------------------------------------------------------------
    {
        auto room  = Scene::Object::Create<Object>();
        auto model = room->AddComponent<ComponentModel>("data/AutoChess/Model/Stage/Room.mv1");
        room->SetScaleAxisXYZ(2.0f);
        room->SetTranslate(float3(-0.5f, -20.0f, 0.0f));
    }
    //---------------------------------------------------------------------------------
    // 駒を生成
    //---------------------------------------------------------------------------------
    {
        auto piece = Scene::Object::Create<Object>();
        piece->SetTranslate(float3(2.0f, 0.0f, -2.0f));
        auto model = piece->AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaGaja.mv1");
        model->SetScaleAxisXYZ(0.02f);
        //---------------------------------------------------------------------------------
        // 駒に回転させたい
        //---------------------------------------------------------------------------------
        auto update_proc = [piece]() { piece->AddRotationAxisXYZ(float3(0.0f, 1.0f, 0.0f)); };
        piece->SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
    //---------------------------------------------------------------------------------
    // スタートボタン
    //---------------------------------------------------------------------------------
    auto start_button = Scene::Object::Create<UIButton>();
    start_button->SetImage(ImageBuffer::GetImageHandle("deff"));
    start_button->SetTranslate(float3(200.0f, 500.0f, 0.0f));
    //---------------------------------------------------------------------------------
    // クリック処理を入れる
    //---------------------------------------------------------------------------------
    {
        auto click_func = []() {
            Scene::Change(Scene::GetScene<InGameScene>());    //シーンの変更を行う処理
        };
        start_button->SetClickFunc(click_func);
    }
    return true;
}

//---------------------------------------------------------------------------
//! @brief 更新処理
//---------------------------------------------------------------------------
void TitleScene::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------
//! @brief 描画処理
//---------------------------------------------------------------------------
void TitleScene::Draw()
{
    __super::Draw();
    //---------------------------------------------------------------------------
    // ボードをビタ描き
    //---------------------------------------------------------------------------
    for(int f = 0; f < 8; f++) {
        for(int r = 0; r < 8; r++) {
            int color = GetColor(0, 0, 0);
            //ファイルとランクの合計値が偶数なら白に
            if(((f + r) % 2) == 0) {
                color = GetColor(255, 255, 255);
            }
            float  x  = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
            float  z  = (f * SQUARE_SIZE) - (4 * SQUARE_SIZE);
            float3 p1 = float3(x + -SQUARE_HALF, -0.1f, z + -SQUARE_HALF);
            float3 p2 = float3(x + SQUARE_HALF, 0.1f, z + SQUARE_HALF);
            DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
        }
    }
}

//---------------------------------------------------------------------------
//! @brief 終了処理
//---------------------------------------------------------------------------
void TitleScene::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------
//! @brief GUI描画
//---------------------------------------------------------------------------
void TitleScene::GUI()
{
    __super::GUI();
}
