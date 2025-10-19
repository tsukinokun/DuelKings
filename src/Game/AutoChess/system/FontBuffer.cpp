//---------------------------------------------------------------------------
//!	@file	FontBuffer.h
//! @brief	フォントのバッファクラス
//---------------------------------------------------------------------------
#include "FontBuffer.h"

std::unordered_map<FontKey, int> FontBuffer::font_buffer_ = {};    // フォントハンドルのバッファを初期化

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
void FontBuffer::Init()
{
    font_buffer_.clear();    // フォントハンドルのバッファをクリア
}

//---------------------------------------------------------------------------------
//!	終了処理
//---------------------------------------------------------------------------------
void FontBuffer::Exit()
{
    // フォントハンドルを解放
    for(auto& pair : font_buffer_) {
        DeleteFontToHandle(pair.second);    //解放
    }
    font_buffer_.clear();    // フォントハンドルのバッファも無効化
}

//---------------------------------------------------------------------------------
//!	フォントハンドルを取得する関数
//---------------------------------------------------------------------------------
int FontBuffer::GetFontHandle(const std::string& font_name, int size, int thickness, int type, int edge_size)
{
    FontKey key(font_name, size, thickness, type, edge_size);
    //バッファにあれば使いまわす
    auto it = font_buffer_.find(key);
    if(it != font_buffer_.end()) {
        return it->second;
    }

    //なければ新規作成
    int handle = CreateFontToHandle(font_name.c_str(), size, thickness, type, -1, edge_size);
    if(handle != -1) {
        font_buffer_[key] = handle;
    }
    return handle;
}
