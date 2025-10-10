//---------------------------------------------------------------------------
//!	@file	ImageBuffer.h
//! @brief	画像のバッファクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

class ImageBuffer
{
public:
    static void Init();    //初期化処理
    static void Exit();    //終了処理

    //画像ハンドルを取得する関数
    //! @param [in] 取得する画像のキー
    //! @retval 画像ハンドル
    static int GetImageHandle(const std::string& image_key);

private:
    static std::unordered_map<std::string, int> image_buffer_;    //画像ハンドルのバッファ
};
