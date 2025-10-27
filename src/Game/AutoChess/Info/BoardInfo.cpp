//---------------------------------------------------------------------------
//!	@file	BoardInfo.cpp
//! @brief	ピースの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "BoardInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------
//! オーナーの名前を設定
//---------------------------------------------------------------------------
void BoardInfo::SetOwner(std::weak_ptr<Agent> owner)
{
    owner_ = owner;
}
//---------------------------------------------------------------------------
//! マスのウィークポインタを取得
//---------------------------------------------------------------------------
std::array<std::array<PieceInfo, 8>, 4> BoardInfo::GetSquarePtrArray() const
{
    return squares_;
}

//---------------------------------------------------------------------------
//! マスに置かれている駒の数を取得
//---------------------------------------------------------------------------
int BoardInfo::GetPieceNumOnSquares() const
{
    // 駒の数をカウント
    int piece_count = 0;
    for(int file = 0; file < 4; ++file) {
        for(int rank = 0; rank < 8; ++rank) {
            if(squares_[file][rank].GetTypeName() != "") {
                ++piece_count;
            }
        }
    }
    return piece_count;
}
//---------------------------------------------------------------------------
//! 駒をマスに追加
//---------------------------------------------------------------------------
void BoardInfo::AddPiece(int file, int rank, PieceInfo piece)
{
    // 駒を配置
    squares_[file][rank] = piece;
}

//---------------------------------------------------------------------------
//! マスの駒を削除
//---------------------------------------------------------------------------
void BoardInfo::RemovePiece(int file, int rank)
{
    // 駒を除去
    squares_[file][rank].SetTypeName("");
}
