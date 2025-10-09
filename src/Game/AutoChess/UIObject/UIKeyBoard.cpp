//---------------------------------------------------------------------------
//!	@file	UIKeyBoard.cpp
//! @brief	UIのキーボードオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIKeyBoard.h"
#include <System/UIComponent/ComponentKeyBoardUI.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIKeyBoard::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	キーボード機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto keyboard_comp  = AddComponent<ComponentKeyBoardUI>();    // 文字列機能コンポーネントを追加
    keyboard_component_ = keyboard_comp;                          // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------------
//!	更新
//---------------------------------------------------------------------------------
void UIKeyBoard::Update()
{
    __super::Update();
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void UIKeyBoard::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void UIKeyBoard::Exit()
{
    __super::Exit();
}

//!GUI表示
void UIKeyBoard::GUI()
{
    __super::GUI();
}

//---------------------------------------------------------------------------------
//! サイズの設定
//---------------------------------------------------------------------------------
std::shared_ptr<UIKeyBoard> UIKeyBoard::SetSize(const float2& size)
{
    if(auto keyboard = keyboard_component_.lock()) {
        keyboard->SetSize(size);
    }
    return dynamic_pointer_cast<UIKeyBoard>(shared_from_this());
}

//---------------------------------------------------------------------------------
//! キーボードのテキスト設定
//---------------------------------------------------------------------------------
std::shared_ptr<UIKeyBoard> UIKeyBoard::SetText(const std::string& text)
{
    if(auto keyboard = keyboard_component_.lock()) {
        keyboard->SetText(text);
    }
    return dynamic_pointer_cast<UIKeyBoard>(shared_from_this());
}
//---------------------------------------------------------------------------------
//! フレームのずらしを設定
//---------------------------------------------------------------------------------
std::shared_ptr<UIKeyBoard> UIKeyBoard::SetFrameOffset(int offset)
{
    if(auto keyboard = keyboard_component_.lock()) {
        keyboard->SetFrameOffset(offset);
    }
    return dynamic_pointer_cast<UIKeyBoard>(shared_from_this());
}
