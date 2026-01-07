//---------------------------------------------------------------------------
//!	@file	SynergyModifierData.h
//! @brief	シナジーごとのモディファイアデータをマッピングしたクラス実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SynergyModifierData.h"
//--------------------------------------------------
//! @brief 初期化関数
//--------------------------------------------------
void SynergyModifierData::Init()
{
}

//--------------------------------------------------
//! @brief シナジーIDに対応するモディファイアデータを取得
//--------------------------------------------------
const std::vector<PieceStatusModifier>& SynergyModifierData::GetModifierData(SynergyID id)
{
    return modifier_status_table_[id];
}
