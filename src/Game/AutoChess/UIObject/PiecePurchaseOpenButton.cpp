//---------------------------------------------------------------------------
//!	@file	PiecePurchaseOpenButton.cpp
//! @brief	ピース購入画面を開くボタンオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "PiecePurchaseOpenButton.h"
#include <System/Component/Component.h>
#include <System/UIComponent/ComponentImage.h>
#include <System/UIComponent/ComponentButton.h>
#include <Game/AutoChess/UIObject/UIAnimation.h>
#include <Game/AutoChess/system/ImageBuffer.h>
#include <Game/AutoChess/system/HlslppUseful.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool PiecePurchaseOpenButton::Init()
{
    __super::Init();
    SetName("PiecePurchaseOpenButton");    //名前設定
    if(auto image_comp = image_component_.lock()) {
        image_comp->SetImage(ImageBuffer::GetImageHandle("piece_purchase_open_button"));    // 画像コンポーネントに画像を設定
    }
    SetScaleAxisXYZ(0.6f);                          //大きさを少し小さく設定
    SetTranslate(float3(1300.0f, 750.0f, 0.0f));    //位置を画面右下あたりに設定
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void PiecePurchaseOpenButton::Update()
{
    __super::Update();
    //マウスが触れていたら
    if(IsMouseOver()) {
        //左マウスクリックを促す
        if(auto click_ui = Scene::Object::Get<UIAnimation>("left_click_anim")) {
            //マウス座標を取得
            float2 mouse_pos = GetMouseFloat2();
            click_ui->SetTranslate(float3(mouse_pos.x, mouse_pos.y, 0.0f));    //位置をマウス座標に設定
        }
        else {
            click_ui = Scene::Object::Create<UIAnimation>();    //左クリックアニメーションオブジェクトを生成
            //マウス座標を取得
            float2 mouse_pos = GetMouseFloat2();
            click_ui->SetTranslate(float3(mouse_pos.x, mouse_pos.y, 0.0f));                          //位置をマウス座標に設定
            click_ui->SetAnimStatus(ImageBuffer::GetImageHandle("left_click_anim"), 2, 0.2f, 30);    //アニメーションステータス設定
            click_ui->SetName("left_click_anim");                                                    //名前設定
        }
    }
    else {
        //非表示
        if(auto click_ui = Scene::Object::Get<UIAnimation>("left_click_anim")) {
            Scene::Object::Release(click_ui);
        }
    }
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void PiecePurchaseOpenButton::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void PiecePurchaseOpenButton::Exit()
{
    __super::Exit();
}

//!GUI表示
void PiecePurchaseOpenButton::GUI()
{
    __super::GUI();
}
