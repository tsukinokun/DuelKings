//---------------------------------------------------------------------------
//!	@file	PieceInfo.cpp
//! @brief	ピースの情報クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceInfo.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------
//! @brief 駒の種類を取得
//---------------------------------------------------------------------------
std::string PieceInfo::GetTypeName() const
{
    return type_name_;
}

//---------------------------------------------------------------------------
//! @brief 所属エージェントを取得（有効なら）
//---------------------------------------------------------------------------
std::shared_ptr<Agent> PieceInfo::GetOwner() const
{
    return owner_agent_.lock();
}

//---------------------------------------------------------------------------
//! @brief 所属エージェントを設定
//---------------------------------------------------------------------------
std::shared_ptr<PieceInfo> PieceInfo::SetOwner(std::weak_ptr<Agent> agent)
{
    owner_agent_ = agent;
    return shared_from_this();
}
