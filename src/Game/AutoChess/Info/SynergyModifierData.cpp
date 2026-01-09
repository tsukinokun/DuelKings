//---------------------------------------------------------------------------
//!	@file	SynergyModifierData.h
//! @brief	シナジーごとのモディファイアデータをマッピングしたクラス実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "SynergyModifierData.h"
//--------------------------------------------------
//! @brief コンストラクタ
//--------------------------------------------------
SynergyModifierData::SynergyModifierData()
{
    //--------------------------------------------------
    // 将棋駒シナジーのモディファイアデータを登録
    //--------------------------------------------------
    {
        PieceStatusModifier shogi_modifier1;
        shogi_modifier1.AddRateAttackPower(0.1f);
        PieceStatusModifier shogi_modifier2;
        shogi_modifier2.AddRateAttackPower(0.3f);
        PieceStatusModifier shogi_modifier3;
        shogi_modifier3.AddRateAttackPower(0.5f);
        modifier_status_table_[SynergyID::Shogi] = {shogi_modifier1, shogi_modifier2, shogi_modifier3};
    }
    //--------------------------------------------------
    // チェス駒シナジーのモディファイアデータを登録
    //--------------------------------------------------
    {
        PieceStatusModifier chess_modifier1;
        chess_modifier1.AddRatePhysicalDefense(0.05f);
        PieceStatusModifier chess_modifier2;
        chess_modifier2.AddRatePhysicalDefense(0.1f);
        PieceStatusModifier chess_modifier3;
        chess_modifier3.AddRatePhysicalDefense(0.25f);
        modifier_status_table_[SynergyID::Chess] = {chess_modifier1, chess_modifier2, chess_modifier3};
    }
    //--------------------------------------------------
    // チャトランガ駒シナジーのモディファイアデータを登録
    //--------------------------------------------------
    {
        PieceStatusModifier chaturanga_modifier1;
        chaturanga_modifier1.AddRateAttackInterval(-0.1f);
        PieceStatusModifier chaturanga_modifier2;
        chaturanga_modifier2.AddRateAttackInterval(-0.2f);
        PieceStatusModifier chaturanga_modifier3;
        chaturanga_modifier3.AddRateAttackInterval(-0.3f);
        modifier_status_table_[SynergyID::Chaturanga] = {chaturanga_modifier1, chaturanga_modifier2, chaturanga_modifier3};
    }
    //--------------------------------------------------
    // マークルック駒シナジーのモディファイアデータを登録
    //--------------------------------------------------
    {
        PieceStatusModifier makruk_modifier1;
        makruk_modifier1.AddRateMagicalDefense(0.1f);
        PieceStatusModifier makruk_modifier2;
        makruk_modifier2.AddRateMagicalDefense(0.15f);
        PieceStatusModifier makruk_modifier3;
        makruk_modifier3.AddRateMagicalDefense(0.25f);
        modifier_status_table_[SynergyID::Makruk] = {makruk_modifier1, makruk_modifier2, makruk_modifier3};
    }
}

//--------------------------------------------------
//! @brief シナジーIDに対応するモディファイアデータを取得
//--------------------------------------------------
const std::vector<PieceStatusModifier>& SynergyModifierData::GetModifierData(SynergyID id)
{
    return modifier_status_table_[id];
}
