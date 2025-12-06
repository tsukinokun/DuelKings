//---------------------------------------------------------------------------
//!	@file	PieceStatusModifier.h
//! @brief
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Agent;    // 前方宣言
class PieceStatusModifier
{
public:
private:
    int   add_max_hp_  = 0;          // 最大HP
    float rate_max_hp_ = 0.0f;       // 最大HP(%)
    int   add_attack_power_;         // 攻撃力
    float rate_attack_power_;        // 攻撃力(%)
    float add_attack_interval_;      // 攻撃間隔(秒)
    float rate_attack_interval_;     // 攻撃間隔(秒%)
    int   add_physical_defense_;     // 物理防御力
    int   rate_physical_defense_;    // 物理防御力(%)
    float add_magical_defense_;      // 魔法防御力
    float rate_magical_defense_;     // 魔法防御力(%)
};
