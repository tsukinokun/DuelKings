//---------------------------------------------------------------------------
//!	@file	ButtonString.cpp
//! @brief	文字列ボタンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ButtonString.h"
#include <Game/AutoChess/system/HlslppUseful.h>
//---------------------------------------------------------------------------------
//	四角同士の当たり判定（座標と大きさに Float2 を渡すバージョン）
//---------------------------------------------------------------------------------
bool CheckBoxCenterHit(const float2& box_pos1, const float2& box_size1, const float2& box_pos2, const float2& box_size2)
{
    if(box_pos1.x + box_size1.x / 2 >= box_pos2.x - box_size2.x / 2 && box_pos1.x - box_size1.x / 2 <= box_pos2.x + box_size2.x / 2) {
        if(box_pos1.y + box_size1.y / 2 >= box_pos2.y - box_size2.y / 2 && box_pos1.y - box_size1.y / 2 <= box_pos2.y + box_size2.y / 2) {
            return true;
        }
    }
    return false;
}

//コンストラクタ
//第一引数:string 文字列
ButtonString::ButtonString(const std::string& str, const float2& pos, bool enable_flag)
{
    str_       = str;
    pos_       = pos;
    is_enable_ = enable_flag;
    size_.x    = GetDrawStringWidth(str.c_str(), str.size());
    size_.y    = GetFontSize();
}

//更新処理
void ButtonString::Update()
{
}

//描画処理
void ButtonString::Render()
{
    if(!is_enable_)
        return;
    int    color = GetColor(255, 255, 255);
    float2 mouse = GetMouseFloat2();
    float2 mouse_size(1.0f, 1.0f);
    if(CheckBoxCenterHit(pos_, size_, mouse, mouse_size)) {
        color = GetColor(255, 255, 0);
        SetFontSize(24);
    }
    int x = pos_.x - (GetDrawStringWidth(str_.c_str(), str_.size()) / 2);
    int y = pos_.y - (GetFontSize() / 2);
    DrawStringF(x, y, str_.c_str(), color);
    SetFontSize(18);
}

//クリックしたらtrueを返す
bool ButtonString::IsClick()
{
    if(!is_enable_)
        return false;
    float2 mouse = GetMouseFloat2();
    float2 mouse_size(1.0f, 1.0f);
    if(CheckBoxCenterHit(pos_, size_, mouse, mouse_size)) {
        if((IsKeyDown(MOUSE_INPUT_LEFT))) {
            return true;
        }
    }
    return false;
}

//有効化を変更
void ButtonString::ChangeEnableFlag(bool enable_flag)
{
    is_enable_ = enable_flag;
}
