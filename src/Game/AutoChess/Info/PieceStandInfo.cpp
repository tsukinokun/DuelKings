//---------------------------------------------------------------------------
//!	@file	PieceStandInfo.cpp
//! @brief	ピーススタンドの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceStandInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//----------------------------------------------------------------------------
// デフォルトコンストラクタ
//----------------------------------------------------------------------------
PieceStandInfo::PieceStandInfo()
{
}
//---------------------------------------------------------------------------
//! ピースの初期化
//---------------------------------------------------------------------------
std::shared_ptr<PieceStandInfo> PieceStandInfo::AddPiece(std::shared_ptr<PieceInfo> piece)
{
    // ピースを確認
    for(auto& piece_wp : pieces_) {
        // 空きがある場合、ピースを追加
        if(piece_wp.expired()) {
            piece_wp = piece;
            break;
        }
    }
    // 置き場が満タンなら追加されないです。
    return shared_from_this();
}

//---------------------------------------------------------------------------
//! オーナーのを設定
//---------------------------------------------------------------------------
std::shared_ptr<PieceStandInfo> PieceStandInfo::SetOwner(std::weak_ptr<Agent> owner_agent)
{
    owner_agent_ = owner_agent;
    return shared_from_this();
}

//---------------------------------------------------------------------------
//! マスのウィークポインタを取得
//---------------------------------------------------------------------------
std::array<std::weak_ptr<PieceInfo>, 8> PieceStandInfo::GetStandPieces() const
{
    return pieces_;
}
//---------------------------------------------------------------------------
//! ピースのスタンドが満タンかどうかを取得する関数
//---------------------------------------------------------------------------
bool PieceStandInfo::IsFull() const
{
    for(const auto& piece_wp : pieces_) {
        if(piece_wp.expired()) {
            return false;    // 空きがある場合、満タンではない
        }
    }
    return true;    // 全てのピースが存在する場合、満タン
}
