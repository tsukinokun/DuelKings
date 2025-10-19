//---------------------------------------------------------------------------
//!	@file	UIText.cpp
//! @brief	UIの文字オブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include "UIText.h"
#include <System/UIComponent/ComponentText.h>

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool UIText::Init()
{
    __super::Init();
    //---------------------------------------------------------------------------------
    //	文字列機能コンポーネントの追加
    //---------------------------------------------------------------------------------
    auto text_comp  = AddComponent<ComponentText>();    // 文字列機能コンポーネントを追加
    text_component_ = text_comp;                        // weak_ptrとして保持
    return true;
}

//---------------------------------------------------------------------------------
//!	フォントの論理名の設定
//---------------------------------------------------------------------------------
std::shared_ptr<UIText> UIText::SetFontName(const std::string& font_name)
{
    if(auto text_comp = text_component_.lock()) {
        text_comp->SetFontName(font_name);
    }
    return dynamic_pointer_cast<UIText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列の設定
//---------------------------------------------------------------------------
std::shared_ptr<UIText> UIText::SetText(const std::string_view& str)
{
    if(auto text_comp = text_component_.lock()) {
        text_comp->SetText(str);    // 文字列コンポーネントに文字列を設定
    }
    return dynamic_pointer_cast<UIText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	文字列色の設定
//---------------------------------------------------------------------------
std::shared_ptr<UIText> UIText::SetColor(int text_color, int edge_color)
{
    if(auto text_comp = text_component_.lock()) {
        text_comp->SetColor(text_color, edge_color);    // 文字列コンポーネントに色を設定
    }
    return dynamic_pointer_cast<UIText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	フォントサイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<UIText> UIText::SetFontSize(int font_size)
{
    if(auto text_comp = text_component_.lock()) {
        text_comp->SetFontSize(font_size);    // フォントサイズを設定
    }
    return dynamic_pointer_cast<UIText>(shared_from_this());
}

//---------------------------------------------------------------------------
//! @brief	エッジサイズの設定
//---------------------------------------------------------------------------
std::shared_ptr<UIText> UIText::SetEdgeSize(int edge_size)
{
    if(auto text_comp = text_component_.lock()) {
        text_comp->SetEdgeSize(edge_size);    // フォントサイズを設定
    }
    return dynamic_pointer_cast<UIText>(shared_from_this());
}
