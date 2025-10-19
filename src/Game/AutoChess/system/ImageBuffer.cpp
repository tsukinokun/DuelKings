//---------------------------------------------------------------------------
//!	@file	ImageBuffer.h
//! @brief	画像のバッファクラス
//---------------------------------------------------------------------------
#include "ImageBuffer.h"

std::unordered_map<std::string, int> ImageBuffer::image_buffer_ = {};    // 画像ハンドルのバッファを初期化

//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
void ImageBuffer::Init()
{
    image_buffer_.clear();                                                                                                 // 画像ハンドルのバッファをクリア
    image_buffer_["deff"]                       = DxLib::LoadGraph("data/AutoChess/Image/deff.png");                       // デフォルト画像
    image_buffer_["piece_purchase_open_button"] = DxLib::LoadGraph("data/AutoChess/Image/PiecePurchaseOpenButton.png");    // ピース購入画面を開くUI画像
    image_buffer_["left_click_anim"]            = DxLib::LoadGraph("data/AutoChess/Image/LeftClickAnim.png");              // 左クリックアニメーション画像
    image_buffer_["exp_button"]                 = DxLib::LoadGraph("data/AutoChess/Image/EXPButton.png");                  // 経験値ボタン画像
}

//---------------------------------------------------------------------------------
//!	終了処理
//---------------------------------------------------------------------------------
void ImageBuffer::Exit()
{
    // 画像ハンドルを解放
    for(auto it = image_buffer_.begin(); it != image_buffer_.end(); ++it) {
        if(it->second != -1) {
            DxLib::DeleteGraph(it->second);    // 画像ハンドルを解放
            it->second = -1;                   // ハンドルを無効化
        }
    }
}

//---------------------------------------------------------------------------------
//!	画像ハンドルを取得する関数
//---------------------------------------------------------------------------------
int ImageBuffer::GetImageHandle(const std::string& image_key)
{
    return image_buffer_[image_key];    // 画像ハンドルを返す
}
