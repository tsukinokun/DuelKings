//---------------------------------------------------------------------------
//!	@file	FontBuffer.h
//! @brief	フォントのバッファクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include "FontKey.h"

class FontBuffer
{
public:
    static void Init();    //初期化処理
    static void Exit();    //終了処理

    //フォントハンドルを取得する関数
    //! @param [in] 取得するフォントのキー
    //! @retval フォントハンドル
    static int GetFontHandle(const std::string& font_name, int size, int thickness = 1, int type = DX_FONTTYPE_ANTIALIASING_EDGE, int edge_size = 0);

private:
    static std::unordered_map<FontKey, int> font_buffer_;    //フォントハンドルのバッファ
};
