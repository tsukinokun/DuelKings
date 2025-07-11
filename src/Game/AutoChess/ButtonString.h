//---------------------------------------------------------------------------
//!	@file	ButtonString.h
//! @brief	文字列ボタンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#pragma once
class ButtonString
{
private:
    std::string str_;          //文字列
    float2      pos_;          //オブジェクトの座標(画面上の)
    float2      size_;         // サイズ
    bool        is_enable_;    //有効化フラグ

public:
    //コンストラクタ
    //! @param str 文字列
    //! @param pos 位置
    //! @param enable_flag 有効化フラグ
    ButtonString(const std::string& str, const float2& pos, bool enable_flag = true);

    //更新処理
    void Update();

    //描画処理
    void Render();

    //クリックされたかを返す
    //! @return クリックされた場合はtrue
    bool IsClick();

    //有効化を変更
    //! @param enable_flag 有効化フラグ
    void ChangeEnableFlag(bool enable_flag);
};
