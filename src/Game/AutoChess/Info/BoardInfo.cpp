//---------------------------------------------------------------------------
//!	@file	BoardInfo.cpp
//! @brief	ピースの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "BoardInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------
//オーナーの名前を設定
//! @param owner [in] オーナー
//! @retval 自身のポインタ
//---------------------------------------------------------------------------
std::shared_ptr<BoardInfo> BoardInfo::SetOwner(std::weak_ptr<Object> owner)
{
    owner_ = owner;
    return shared_from_this();
}
//---------------------------------------------------------------------------
//マスのウィークポインタを取得
//! @retval マスのウィークポインタ
//---------------------------------------------------------------------------
std::array<std::array<std::weak_ptr<PieceInfo>, 4>, 8> BoardInfo::GetSquarePtrArray() const
{
    return squares_;
}

//---------------------------------------------------------------------------
//マスに置かれている駒の数を取得
//! @retval 駒の数
//---------------------------------------------------------------------------
int BoardInfo::GetPieceNumOnSquares() const
{
    return piece_count_;    // キャッシュを返す
}
//---------------------------------------------------------------------------
// 駒をマスに追加
//! @param file [in] ファイル（列）
//! @param rank [in] ランク（行）
//! @param piece [in] 追加する駒の共有ポインタ
//! @retval 自身のポインタ
//---------------------------------------------------------------------------
std::shared_ptr<BoardInfo> BoardInfo::AddPiece(int file, int rank, std::shared_ptr<PieceInfo> piece)
{
    if(!piece)
        return shared_from_this();    // 無効なポインタは無視

    // 既存のマスに駒がいなければカウントを増やす
    if(squares_[file][rank].expired()) {
        piece_count_++;
    }

    // 駒を配置
    squares_[file][rank] = piece;
    return shared_from_this();
}

//---------------------------------------------------------------------------
// 駒をマスに追加
//! @param file [in] ファイル（列）
//! @param rank [in] ランク（行）
//! @retval 自身のポインタ
//---------------------------------------------------------------------------
std::shared_ptr<BoardInfo> BoardInfo::RemovePiece(int file, int rank)
{
    // 駒が存在していればカウントを減らす
    if(!squares_[file][rank].expired()) {
        piece_count_--;
    }

    // 駒を除去
    squares_[file][rank].reset();
    return shared_from_this();
}
