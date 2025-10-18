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
    SetTranslate(float3(1150.0f, 600.0f, 0.0f));    //位置を画面右下あたりに設定
    //左クリックを促す
    SetOverInformation(ComponentButton::OverInformation::LEFT_CLICK);
    return true;
}
