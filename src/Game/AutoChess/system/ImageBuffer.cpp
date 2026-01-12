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
    image_buffer_["gold_icon"]                  = DxLib::LoadGraph("data/AutoChess/Image/GoldIcon.png");                   // ゴールドアイコン画像
    image_buffer_["gold_detail_back"]           = DxLib::LoadGraph("data/AutoChess/Image/GoldDetailBack.png");             // ゴールド詳細背景画像
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
        DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/ChaturangaSynergyIcon.png");                                    // チャトランガシナジーアイコン画像
    image_buffer_["makruk_synergy_icon"]  = DxLib::LoadGraph("data/AutoChess/Image/SynergyIcons/MakrukSynergyIcon.png");    // マークルックシナジーアイコン画像
    image_buffer_["level_up"]             = DxLib::LoadGraph("data/AutoChess/Image/LevelUP.png");                           // 追加画像
    image_buffer_["sell_button"]          = DxLib::LoadGraph("data/AutoChess/Image/SellButton.png");                        // 売却ボタン画像
    image_buffer_["piece_detail_back"]    = DxLib::LoadGraph("data/AutoChess/Image/PieceDetailBack.png");                   // ピース詳細背景画像
    image_buffer_["attack_power_icon"]    = DxLib::LoadGraph("data/AutoChess/Image/AttackPowerIcon.png");                   // 攻撃力アイコン画像
    image_buffer_["attack_range_icon"]    = DxLib::LoadGraph("data/AutoChess/Image/AttackRangeIcon.png");                   // 攻撃範囲アイコン画像
    image_buffer_["attack_interval_icon"] = DxLib::LoadGraph("data/AutoChess/Image/AttackIntervalIcon.png");                // 攻撃間隔アイコン画像
    image_buffer_["japanese_chess_pawn_icon"]   = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessPawnIcon.png");      //将棋の歩兵アイコン画像
    image_buffer_["japanese_chess_lance_icon"]  = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessLanceIcon.png");     //将棋の香車アイコン画像
    image_buffer_["japanese_chess_knight_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessKnightIcon.png");    //将棋の桂馬アイコン画像
    image_buffer_["japanese_chess_silver_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessSilverIcon.png");    //将棋の銀アイコン画像
    image_buffer_["japanese_chess_gold_icon"]   = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessGoldIcon.png");      //将棋の金アイコン画像
    image_buffer_["japanese_chess_bishop_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessBishopIcon.png");    //将棋の角アイコン画像
    image_buffer_["japanese_chess_rook_icon"]   = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessRookIcon.png");      //将棋の飛車アイコン画像
    image_buffer_["japanese_chess_king_icon"]   = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/JapaneseChessKingIcon.png");      //将棋の王アイコン画像
    image_buffer_["chess_pawn_icon"]            = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessPawnIcon.png");      // チェスのポーンアイコン画像
    image_buffer_["chess_bishop_icon"]          = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessBishopIcon.png");    // チェスのビショップアイコン画像
    image_buffer_["chess_knight_icon"]          = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessKnightIcon.png");    // チェスのナイトアイコン画像
    image_buffer_["chess_rook_icon"]            = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessRookIcon.png");      // チェスのルークアイコン画像
    image_buffer_["chess_king_icon"]            = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessKingIcon.png");      // チェスのキングアイコン画像
    image_buffer_["chess_queen_icon"]           = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChessQueenIcon.png");     // チェスのクイーンアイコン画像
    image_buffer_["chaturanga_padati_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaPadatiIcon.png");                                      // チャトランガのパダチアイコン画像
    image_buffer_["chaturanga_asva_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaAsvaIcon.png");    // チャトランガのアスヴァアイコン画像
    image_buffer_["chaturanga_gaja_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaGajaIcon.png");    // チャトランガのガジャアイコン画像
    image_buffer_["chaturanga_mantri_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaMantriIcon.png");    // チャトランガのマントリアイコン画像
    image_buffer_["chaturanga_ratha_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaRathaIcon.png");    // チャトランガのラタアイコン画像
    image_buffer_["chaturanga_raja_icon"]  = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/ChaturangaRajaIcon.png");     // チャトランガのラジャアイコン画像
    image_buffer_["makruk_bia_icon"]       = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukBiaIcon.png");          // マークルックのビアアイコン画像
    image_buffer_["makruk_met_icon"]       = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukMetIcon.png");          // マークルックのメットアイコン画像
    image_buffer_["makruk_khon_icon"]      = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukKhonIcon.png");         // マークルックのコーンアイコン画像
    image_buffer_["makruk_ma_icon"]        = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukMaIcon.png");           // マークルックのマーアイコン画像
    image_buffer_["makruk_ruea_icon"]      = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukRueaIcon.png");         // マークルックのルアアイコン画像
    image_buffer_["makruk_khun_icon"]      = DxLib::LoadGraph("data/AutoChess/Image/PieceIcons/MakrukKhunIcon.png");         // マークルックのクンアイコン画像

    image_buffer_["physical_defense_icon"] = DxLib::LoadGraph("data/AutoChess/Image/PhysicalDefenceIcon.png");    // 物理防御力アイコン画像
    image_buffer_["magical_defense_icon"]  = DxLib::LoadGraph("data/AutoChess/Image/MagicalDefenceIcon.png");     // 物理防御力アイコン画像
    image_buffer_["japanese_chess_pawn_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessPawnSkillIcon.png");    // 将棋の歩兵スキルアイコン画像
    image_buffer_["japanese_chess_lance_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessLanceSkillIcon.png");    // 将棋の香車スキルアイコン画像
    image_buffer_["japanese_chess_knight_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessLanceSkillIcon.png");    // 将棋の桂馬スキルアイコン画像
    image_buffer_["japanese_chess_silver_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessSilverSkillIcon.png");    // 将棋の銀スキルアイコン画像
    image_buffer_["japanese_chess_gold_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessGoldSkillIcon.png");    // 将棋の金スキルアイコン画像
    image_buffer_["japanese_chess_bishop_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessBishopSkillIcon.png");    // 将棋の角スキルアイコン画像
    image_buffer_["japanese_chess_rook_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessRookSkillIcon.png");    // 将棋の飛車スキルアイコン画像
    image_buffer_["japanese_chess_king_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/JapaneseChessKingSkillIcon.png");                                 // 将棋の王スキルアイコン画像
    image_buffer_["chess_pawn_skill_icon"] = DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessPawnSkillIcon.png");    // チェスのポーンスキルアイコン画像
    image_buffer_["chess_bishop_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessBishopSkillIcon.png");    // チェスのビショップスキルアイコン画像
    image_buffer_["chess_knight_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessKnightSkillIcon.png");                                       // チェスのナイトスキルアイコン画像
    image_buffer_["chess_rook_skill_icon"] = DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessRookSkillIcon.png");    // チェスのルークスキルアイコン画像
    image_buffer_["chess_king_skill_icon"] = DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessKingSkillIcon.png");    // チェスのキングスキルアイコン画像
    image_buffer_["chess_queen_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChessQueenSkillIcon.png");    // チェスのクイーンスキルアイコン画像
    image_buffer_["chaturanga_padati_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaPadatiSkillIcon.png");    // チャトランガのパダチスキルアイコン画像
    image_buffer_["chaturanga_asva_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaAsvaSkillIcon.png");    // チャトランガのアスヴァスキルアイコン画像
    image_buffer_["chaturanga_gaja_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaGajaSkillIcon.png");    // チャトランガのガジャスキルアイコン画像
    image_buffer_["chaturanga_mantri_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaMantriSkillIcon.png");    // チャトランガのマントリスキルアイコン画像
    image_buffer_["chaturanga_ratha_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaRathaSkillIcon.png");    // チャトランガのラタスキルアイコン画像
    image_buffer_["chaturanga_raja_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/ChaturangaRajaSkillIcon.png");    // チャトランガのラジャスキルアイコン画像
    image_buffer_["makruk_bia_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukBiaSkillIcon.png");    // マークルックのビアスキルアイコン画像
    image_buffer_["makruk_met_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukMetSkillIcon.png");    // マークルックのメットスキルアイコン画像
    image_buffer_["makruk_khon_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukKhonSkillIcon.png");    // マークルックのコーンスキルアイコン画像
    image_buffer_["makruk_ma_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukMaSkillIcon.png");    // マークルックのマースキルアイコン画像
    image_buffer_["makruk_ruea_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukRueaSkillIcon.png");    // マークルックのルアスキルアイコン画像
    image_buffer_["makruk_khun_skill_icon"] =
        DxLib::LoadGraph("data/AutoChess/Image/SkillIcons/MakrukKhunSkillIcon.png");    // マークルックのクンスキルアイコン画像
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
