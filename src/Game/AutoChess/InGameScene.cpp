//---------------------------------------------------------------------------
//!	@file	InGameScene.cpp
//! @brief	オートチェスのインゲームシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "InGameScene.h"
#include "Camera.h"
#include "ChessBoard.h"
#include "PieceStand.h"
#include "Agent.h"
#include "Player.h"
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/system/MouseRay.h>
#include <Game/AutoChess/ShopStand.h>
#include <Game/AutoChess/UIObject/PiecePurchaseOpenButton.h>
#include <Game/AutoChess/UIObject/UIText.h>
#include <System/UIComponent/ComponentTransformUI.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool InGameScene::Init()
{
    __super::Init();
    ImageBuffer::Init();                                 //画像バッファの初期化
    Scene::Object::Create<Camera>();                     //カメラ
    Scene::Object::Create<Player>();                     //プレイヤー
    Scene::Object::Create<MouseRay>();                   //マウス光線
    Scene::Object::Create<PiecePurchaseOpenButton>();    //ピース購入画面を開けるボタン
    //---------------------------------------------------------------------------------
    //  経験値ボタン
    //---------------------------------------------------------------------------------
    auto exp_button = Scene::Object::Create<UIButton>();    //経験値ボタン
    exp_button->SetImage(ImageBuffer::GetImageHandle("exp_button"));
    exp_button->SetScaleAxisXYZ(0.6f);                         //大きさを少し小さく設定
    exp_button->SetTranslate(float3(150.0f, 600.0f, 0.0f));    //位置を画面右下あたりに設定
    //左クリックを促す
    exp_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
    //---------------------------------------------------------------------------------
    //  駒数制限UI
    //---------------------------------------------------------------------------------
    auto piece_max_ui = Scene::Object::Create<UIText>();
    piece_max_ui->SetFontSize(80);                                         //フォントサイズ設定
    piece_max_ui->SetColor(GetColor(0, 0, 0), GetColor(255, 255, 255));    //文字色設定
    //更新処理
    auto set_text_proc = [piece_max_ui]() {
        auto player = Scene::Object::Get<Player>();
        int  level  = player->GetAgentLevel();
        piece_max_ui->SetText(std::to_string(level));
    };
    piece_max_ui->SetProc("set_level", set_text_proc, ProcTiming::Update, ProcPriority::NONE);
    piece_max_ui->SetTranslate(float3(700.0f, 250.0f, 0.0f));
    piece_max_ui->SetAlignment(ComponentTransformUI::Alignment::MiddleCenter);
    //---------------------------------------------------------------------------------
    //  購入ボタン
    //---------------------------------------------------------------------------------
    auto piece_purchase_button = Scene::Object::Create<UIButton>();
    auto texture               = std::make_shared<Texture>(100, 200, DXGI_FORMAT_R8G8B8A8_UNORM);

    //ターゲットをうつす処理を入れ込む。
    auto draw_target = [piece_purchase_button, texture]() {
        //とりあえずプレイヤーを取得
        if(auto shop_stand = Scene::Object::Get<ShopStand>()) {
            auto shop_pieces = shop_stand->GetShopPieces();
            SetRenderTarget(texture.get(), nullptr);
            piece_purchase_button->SetImage(*texture);    //スクリーンを入れ込む。
            ////戻す
            SetRenderTarget(GetHdrBuffer(), GetDepthStencil());
        }
    };
    piece_purchase_button->SetProc("draw_target", draw_target, ProcTiming::Draw, ProcPriority::NONE);
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void InGameScene::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void InGameScene::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void InGameScene::Exit()
{
    ImageBuffer::Exit();    //画像バッファの終了
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void InGameScene::GUI()
{
    __super::GUI();
}
