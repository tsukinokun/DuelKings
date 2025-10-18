//---------------------------------------------------------------------------
//!	@file	UIButton.cpp
//! @brief	UIのボタンオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIButton.h"
#include <System/UIComponent/ComponentImage.h>
#include <System/UIComponent/ComponentButton.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIButton::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	文字列機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto img_comp     = AddComponent<ComponentImage>();     // 文字列機能コンポーネントを追加
    image_component_  = img_comp;                           // weak_ptrとして保持
    auto btn_comp     = AddComponent<ComponentButton>();    // ボタンコンポーネントを追加
    button_component_ = btn_comp;                           // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------
//! @brief	画像の設定
//---------------------------------------------------------------------------
std::shared_ptr<UIButton> UIButton::SetImage(int image)
{
    if(auto image_comp = image_component_.lock()) {
        image_comp->SetImage(image);    // 画像コンポーネントに画像を設定
    }
    return dynamic_pointer_cast<UIButton>(shared_from_this());
}
//---------------------------------------------------------------------------
// ボタンがクリックされたかを返す関数
//! @retval クリックされたか
//---------------------------------------------------------------------------
bool UIButton::IsClick() const
{
    if(auto button_comp = button_component_.lock()) {
        return button_comp->IsClick();    // ボタンコンポーネントにクリックされたかを問い合わせる
    }
    return false;
}
//---------------------------------------------------------------------------
//  マウスがボタンに触れているかを返す関数
//! @return マウスがボタンに触れているか
//---------------------------------------------------------------------------
bool UIButton::IsMouseOver() const
{
    if(auto button_comp = button_component_.lock()) {
        return button_comp->IsMouseOver();    // ボタンコンポーネントにマウスが触れているかを問い合わせる
    }
    return false;
}
//---------------------------------------------------------------------------
//  マウスがボタンに触れている時に表示させる情報の設定
//! @return 自身のポインタ
//---------------------------------------------------------------------------
std::shared_ptr<UIButton> UIButton::SetOverInformation(ComponentButton::OverInformation info)
{
    if(auto button_comp = button_component_.lock()) {
        button_comp->SetOverInformation(info);    // ボタンコンポーネントに情報を設定
    }
    return dynamic_pointer_cast<UIButton>(shared_from_this());
}
//---------------------------------------------------------------------------
//画像ハンドルの取得
//---------------------------------------------------------------------------
int UIButton::GetImageHandle()
{
    if(auto image_comp = image_component_.lock()) {
        return image_comp->GetImageHandle();    // 画像コンポーネントから画像ハンドルを取得
    }
    return -1;
}
