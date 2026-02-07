//---------------------------------------------------------------------------
//!	@file	ResultScene.cpp
//! @brief	オートチェスのリザルトシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ResultScene.h"
#include <Game/AutoChess/InGameScene.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Funiture/Glass.h>
#include <system/Component/ComponentModel.h>
#include <Game/AutoChess/UIObject/UIButton.h>
#include <Game/AutoChess/UIObject/UIText.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <System/Component/ComponentCamera.h>
#include <Game/AutoChess/UIObject/UIImage.h>
#include <Game/AutoChess/system/SoundManager.h>
#include <Game/AutoChess/TitleScene.h>
#include <Game/AutoChess/system/GameRepository.h>
#include <System/Component/ComponentFilterFade.h>
#include <System2/Shadowmap.h>
//---------------------------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------------------------
bool ResultScene::Init()
{
    __super::Init();
    Scene::Object::Create<Shadowmap>("Shadowmap");    // シャドウマップ
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
        piece->SetTranslate(float3(0.0f, 0.0f, -2.0f));
        auto model = piece->AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaGaja.mv1");
        model->SetScaleAxisXYZ(0.02f);
        //---------------------------------------------------------------------------------
        // 駒に回転させたい
        //---------------------------------------------------------------------------------
        auto update_proc = [piece]() { piece->AddRotationAxisXYZ(float3(0.0f, 0.2f, 0.0f)); };
        piece->SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    }
    //---------------------------------------------------------------------------------
    // 順位を表示するUI
    //---------------------------------------------------------------------------------
    {
        auto result_text = Scene::Object::Create<UIText>();
        result_text->SetTranslate(float3(WINDOW_W / 2.0f, WINDOW_H / 2.0f - 50.0f, 0.0f));    //位置の設定
        result_text->SetFontName("游明朝");                                                   //フォントの設定
        result_text->SetFontSize(128);                                                        //サイズの設定
        result_text->SetColor(GetColor(255, 255, 255));                                       //色の設定
        int player_rank = GameRepository::instance()->GetPlayerRank();                        //プレイヤーの最終順位を取得
        result_text->SetText(std::to_string(player_rank) + "位");                             //表示するテキストの設定
    }

    //---------------------------------------------------------------------------------
    // マスのモデル(バトル中表示用)の生成
    //---------------------------------------------------------------------------------
    constexpr float SQUARE_SIZE = 1.0f;    //マスの一辺の長さ
    constexpr int   RANK_MAX_   = 8;
    constexpr int   FILE_MAX_   = 8;
    constexpr float RANK_HALF_  = RANK_MAX_ / 2;
    constexpr float FILE_HALF_  = FILE_MAX_ / 2;
    for(int f = 0; f < FILE_MAX_; f++) {
        for(int r = 0; r < RANK_MAX_; r++) {
            auto square = Scene::Object::Create<Object>();
            //ファイルとランクの合計値が偶数なら黒
            bool             is_black   = ((f + r) % 2) == 0;
            std::string_view model_path = "data/AutoChess/Model/Square/WhiteSquare.mv1";
            if(is_black) {
                model_path = "data/AutoChess/Model/Square/BlackSquare.mv1";
            }
            square->AddComponent<ComponentModel>(model_path);
            square->SetScaleAxisXYZ(float3(0.05f, 0.05f, 0.05f));
            float  x = (r * SQUARE_SIZE) - RANK_HALF_ * (SQUARE_SIZE);
            float  z = (f * SQUARE_SIZE) - (FILE_HALF_ * SQUARE_SIZE);
            float3 p = float3(x, -0.1f, z);
            square->SetTranslate(p);
        }
    }

    return true;
}

//---------------------------------------------------------------------------
//! @brief 更新処理
//---------------------------------------------------------------------------
void ResultScene::Update()
{
    __super::Update();
    //サウンドのインスタンスを取得
    auto soundmanager = SoundManager::instance();
    //---------------------------------------------------------------------------------
    // タイトルBGMの再生
    //---------------------------------------------------------------------------------
    //再生中でなければ
    if(!soundmanager->IsPlayingBGM("title")) {
        // タイトルBGMを再生
        soundmanager->PlayBGM("title");
    }

    //---------------------------------------------------------------------------------
    // 何かしらのマウスボタンを押したらスタート
    //---------------------------------------------------------------------------------
    if(GetMouseInput()) {
        soundmanager->StopBGM("title");                  // タイトルBGMを停止
        soundmanager->PlaySE("start");                   // ゲーム開始の効果音を再生
        Scene::Change(Scene::GetScene<TitleScene>());    //シーンの変更を行う処理
    }
}

//---------------------------------------------------------------------------
//! @brief 描画処理
//---------------------------------------------------------------------------
void ResultScene::Draw()
{
    __super::Draw();
    ////---------------------------------------------------------------------------
    //// ボードをビタ描き
    ////---------------------------------------------------------------------------
    //for(int f = 0; f < 8; f++) {
    //    for(int r = 0; r < 8; r++) {
    //        int color = GetColor(0, 0, 0);
    //        //ファイルとランクの合計値が偶数なら白に
    //        if(((f + r) % 2) == 0) {
    //            color = GetColor(255, 255, 255);
    //        }
    //        float  x  = (r * SQUARE_SIZE) - 4 * (SQUARE_SIZE);
    //        float  z  = (f * SQUARE_SIZE) - (4 * SQUARE_SIZE);
    //        float3 p1 = float3(x + -SQUARE_HALF, -0.1f, z + -SQUARE_HALF);
    //        float3 p2 = float3(x + SQUARE_HALF, 0.1f, z + SQUARE_HALF);
    //        DrawCube3D(cast(p1), cast(p2), color, color, TRUE);
    //    }
    //}
}

//---------------------------------------------------------------------------
//! @brief 終了処理
//---------------------------------------------------------------------------
void ResultScene::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------
//! @brief GUI描画
//---------------------------------------------------------------------------
void ResultScene::GUI()
{
    __super::GUI();
}
