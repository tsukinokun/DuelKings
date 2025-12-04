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
    image_buffer_["reroll_button"]              = DxLib::LoadGraph("data/AutoChess/Image/Reroll.png");                     // リロールボタン画像
    image_buffer_["locked_button"]              = DxLib::LoadGraph("data/AutoChess/Image/Locked.png");                     // ロックボタン画像
    image_buffer_["unlocked_button"]            = DxLib::LoadGraph("data/AutoChess/Image/Unlocked.png");                   // アンロックボタン画像
    image_buffer_["level1_star"]                = DxLib::LoadGraph("data/AutoChess/Image/Level1Star.png");                 // レベル1スター画像
    image_buffer_["level2_star"]                = DxLib::LoadGraph("data/AutoChess/Image/Level2Star.png");                 // レベル2スター画像
    image_buffer_["level3_star"]                = DxLib::LoadGraph("data/AutoChess/Image/Level3Star.png");                 // レベル3スター画像
    image_buffer_["japanese_chess_synergy_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/JapaneseChessSynergyIcon.png");                              // 将棋シナジーアイコン画像
    image_buffer_["chess_synergy_icon"] = DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/ChessSynergyIcon.png");    // チェスシナジーアイコン画像
    image_buffer_["chaturanga_synergy_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/ChaturangaSynergyIcon.png");                                     // チャトランガシナジーアイコン画像
    image_buffer_["makruk_synergy_icon"]   = DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/MakrukSynergyIcon.png");    // マークルックシナジーアイコン画像
    image_buffer_["level_up"]              = DxLib::LoadGraph("data/AutoChess/Image/LevelUP.png");                           // 追加画像
    image_buffer_["sell_button"]           = DxLib::LoadGraph("data/AutoChess/Image/SellButton.png");                        // 売却ボタン画像
    image_buffer_["piece_detail_back"]     = DxLib::LoadGraph("data/AutoChess/Image/PieceDetailBack.png");                   // ピース詳細背景画像
    image_buffer_["attack_power_icon"]     = DxLib::LoadGraph("data/AutoChess/Image/AttackPowerIcon.png");                   // 攻撃力アイコン画像
    image_buffer_["attack_range_icon"]     = DxLib::LoadGraph("data/AutoChess/Image/AttackRangeIcon.png");                   // 攻撃範囲アイコン画像
    image_buffer_["attack_interval_icon"]  = DxLib::LoadGraph("data/AutoChess/Image/AttackIntervalIcon.png");                // 攻撃間隔アイコン画像
    image_buffer_["chess_bishop_icon"]     = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessBishopIcon.png");        // チェスのビショップアイコン画像
    image_buffer_["physical_defense_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PhysicalDefenceIcon.png");               // 物理防御力アイコン画像
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
