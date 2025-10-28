//---------------------------------------------------------------------------
//!	@file	Agent.cpp
//! @brief	インゲームシーンのエージェントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include <Game/AutoChess/system/GameConst.h>
#include "Square.h"
#include <Game/AutoChess/Info/BoardInfo.h>
#include <Game/AutoChess/Info/ShopStandInfo.h>
#include <Game/AutoChess/Info/PieceStandInfo.h>
#include <Game/AutoChess/Info/PieceInfo.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Agent::Init()
{
    __super::Init();
    SetName("Agent");
    //---------------------------------------------------------------------------------
    //	ピーススタンドを作成
    //---------------------------------------------------------------------------------
    stand_info_.SetOwner(dynamic_pointer_cast<Agent>(shared_from_this()));
    //---------------------------------------------------------------------------------
    //	ショップスタンドを作成
    //---------------------------------------------------------------------------------
    shop_stand_info_.SetOwner(dynamic_pointer_cast<Agent>(shared_from_this()));
    shop_stand_info_.RerollShopPieces();    //初期ピースをリロール
    //---------------------------------------------------------------------------------
    //	チェスボードを作成
    //---------------------------------------------------------------------------------
    board_info_.SetOwner(dynamic_pointer_cast<Agent>(shared_from_this()));
    return true;
}
//---------------------------------------------------------------------------------
//!	経験値を増やす関数
//---------------------------------------------------------------------------------
std::shared_ptr<Agent> Agent::AddExp(int exp)
{
    exp_ += exp;
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//---------------------------------------------------------------------------------
//! 経験値量から、レベル(置ける駒数)を判定して返す関数
//---------------------------------------------------------------------------------
int Agent::GetAgentLevel()
{
    int level     = 0;
    int total_exp = 0;
    for(int i = 0; i < exp_table.size(); ++i) {
        total_exp += exp_table[i];
        if(exp_ < total_exp)
            break;
        ++level;
    }

    return level;
}
//---------------------------------------------------------------------------------
//! 次のレベルまでに必要な経験値を取得する関数
//---------------------------------------------------------------------------------
int Agent::GetNextLevelExp()
{
    int level     = 0;
    int total_exp = 0;
    for(int i = 0; i < exp_table.size(); ++i) {
        total_exp += exp_table[i];
        if(exp_ < total_exp)
            break;
        ++level;
    }
    if(level < exp_table.size()) {
        return exp_table[level];
    }
    else {
        return 0;    //最大レベルの場合は0を返す
    }
}
//---------------------------------------------------------------------------------
//! 現在の経験値(レベルアップに使用した経験値は抜いた)を取得する関数
//---------------------------------------------------------------------------------
int Agent::GetCurrentExp() const
{
    int level     = 0;
    int total_exp = 0;
    for(int i = 0; i < exp_table.size(); ++i) {
        total_exp += exp_table[i];
        if(exp_ < total_exp)
            break;
        ++level;
    }
    int used_exp = 0;
    for(int i = 0; i < level; ++i) {
        used_exp += exp_table[i];
    }
    return exp_ - used_exp;
}
//---------------------------------------------------------------------------------
//! 置かれているピースの数を取得する関数
//---------------------------------------------------------------------------------
int Agent::GetPlacedPieceNum() const
{
    int placed_piece_num = 0;    // 置かれているピースの数
    placed_piece_num     = board_info_.GetPieceNumOnSquares();
    return placed_piece_num;
}
//---------------------------------------------------------------------------------
//! ショップに並んでいるピースを取得する関数
//---------------------------------------------------------------------------------
std::array<PieceInfo, 5> Agent::GetShopPieces()
{
    std::array<PieceInfo, 5> shop_pieces;
    shop_pieces = shop_stand_info_.GetShopPieces();
    return shop_pieces;
}

//-----------------------------------------------------------
//! スタンドに駒を追加する関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::AddPieceToStand(const PieceInfo& piece)
{
    // スタンドに駒を追加
    stand_info_.AddPiece(piece);
    return dynamic_pointer_cast<Agent>(shared_from_this());
}

//-----------------------------------------------------------
//! 指定インデックスのショップピースを無効化する関数
//-----------------------------------------------------------
bool Agent::InvalidateShopPiece(size_t index)
{
    return shop_stand_info_.InvalidateShopPiece(index);
}
//-----------------------------------------------------------
//! ショップのピースをリロールする関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::RerollShopPieces()
{
    shop_stand_info_.RerollShopPieces();
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//-----------------------------------------------------------
//! ショップのロック状態を切り替える関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::ToggleShopLockState()
{
    //ロック中かを確認して
    if(shop_stand_info_.IsLocked()) {
        shop_stand_info_.SetLockState(false);    // ロック解除
    }
    else {
        shop_stand_info_.SetLockState(true);    // ロック
    }
    return dynamic_pointer_cast<Agent>(shared_from_this());
}

//-----------------------------------------------------------
//! ショップのロック状態を取得する関数
//-----------------------------------------------------------
bool Agent::IsShopLocked() const
{
    return shop_stand_info_.IsLocked();
}
//-----------------------------------------------------------
// ピーススタンドが満タンかどうかを取得する関数
//-----------------------------------------------------------
bool Agent::IsPieceStandFull() const
{
    return stand_info_.IsFull();
}
//-----------------------------------------------------------
//! ピーススタンドの駒情報を設定する関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::SetPieceStandInfo(size_t index, const PieceInfo& piece_info)
{
    // ピーススタンド情報を設定
    stand_info_.GetStandPieces()[index] = piece_info;
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//-----------------------------------------------------------
//! 自陣のチェスボードの駒情報を設定する関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::SetBoardInfo(int file, int rank, const PieceInfo& piece_info)
{
    // チェスボード情報を設定
    board_info_.AddPiece(file, rank, piece_info);
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//-----------------------------------------------------------
//! 自陣のチェスボードの駒情報を削除する関数
//-----------------------------------------------------------
std::shared_ptr<Agent> Agent::RemoveBoardInfo(int file, int rank)
{
    // チェスボード情報を削除
    board_info_.RemovePiece(file, rank);
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//-----------------------------------------------------------
// ピース制限を強制する関数
//-----------------------------------------------------------
void Agent::EnforcePieceLimit()
{
    int level              = GetAgentLevel();        // エージェントのレベルを取得
    int placed_piece_count = GetPlacedPieceNum();    // 置かれているピースの数を取得
    if(placed_piece_count <= level) {
        return;    // レベル以内なら何もしない
    }
    // レベルを超えている場合、超過分のピースを削除
    int pieces_to_remove = placed_piece_count - level;
    for(int file = 0; file < 4 && pieces_to_remove > 0; ++file) {
        for(int rank = 0; rank < 8 && pieces_to_remove > 0; ++rank) {
            PieceInfo piece = board_info_.GetSquarePtrArray()[file][rank];
            if(piece.GetTypeName() != "") {
                board_info_.RemovePiece(file, rank);
                --pieces_to_remove;
            }
        }
    }
}

//-----------------------------------------------------------
//! ピーススタンドとチェスボードの情報を交換する関数
//-----------------------------------------------------------
void Agent::SwapPieceStandAndBoardInfo(size_t piece_stand_index, int board_file, int board_rank)
{
    auto agent = shared_from_this();
    //一時変数にピース情報を保存
    auto stand_piece = stand_info_.GetStandPieces()[piece_stand_index];
    auto board_piece = board_info_.GetSquarePtrArray()[board_file][board_rank];
    // ピーススタンドのピースをチェスボードに移動
    board_info_.AddPiece(board_file, board_rank, stand_piece);
    // チェスボードのピースをピーススタンドに移動
    stand_info_.SetPieceAt(piece_stand_index, board_piece);
}
//-----------------------------------------------------------
//! チェスボードの駒をスタンドに移動する関数
//-----------------------------------------------------------
bool Agent::MoveBoardPieceToStand(int board_file, int board_rank)
{
    // チェスボードのピース情報を取得
    auto board_piece = board_info_.GetSquarePtrArray()[board_file][board_rank];
    if(board_piece.GetTypeName() == "") {
        return false;    // 駒が存在しない場合は何もしない
    }
    // ピーススタンドの空いている場所を探す
    for(size_t i = 0; i < stand_info_.GetStandPieces().size(); ++i) {
        auto& stand_piece = stand_info_.GetStandPieces()[i];
        if(stand_piece.GetTypeName() == "") {
            // 空いている場所が見つかったら、ピースを移動
            stand_piece = board_piece;
            board_info_.RemovePiece(board_file, board_rank);
            return true;
        }
    }
    return false;    //空いてる場所がなかった場合は失敗
}
