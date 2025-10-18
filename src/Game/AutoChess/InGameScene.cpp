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
#include <Game/AutoChess/UIObject/PiecePurchaseOpenButton.h>
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
    exp_button->SetTranslate(float3(250.0f, 750.0f, 0.0f));    //位置を画面右下あたりに設定
    //左クリックを促す
    exp_button->SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
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
