//---------------------------------------------------------------------------
//!	@file	Logic.cpp
//! @brief	ゲームロジック置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Logic.h"
#include <Game/AutoChess/Agent.h>
//---------------------------------------------------------------------------
//! @brief	ラウンド終了時の所持金計算関数
//---------------------------------------------------------------------------
int CalculateRoundGold(const std::shared_ptr<Agent>& agent, bool isWin)
{
    int gold_gain = 5;    // 基本収入

    if(isWin)
        gold_gain += 1;    // 勝利ボーナス

    // 連勝ボーナス
    if(agent->GetWinStreak() >= 3) {
        gold_gain += std::min(3, agent->GetWinStreak() / 2);
    }

    // 連敗ボーナス
    if(agent->GetLoseStreak() >= 3) {
        gold_gain += std::min(3, agent->GetLoseStreak() / 2);
    }

    // 利子
    int interest = agent->GetGold() / 10;
    if(interest > 5)
        interest = 5;
    gold_gain += interest;

    return gold_gain;
}

//---------------------------------------------------------------------------
//! @brief マナ回復量を計算する
//---------------------------------------------------------------------------
int CalculateMPGain(int damage, DamageType damage_type, bool is_taken_damage)
{
    if(is_taken_damage) {
        return std::min(damage / 5, 50);    // 被ダメージ
    }

    switch(damage_type) {
    case DamageType::Physical:
        return std::min(damage, 10);    // 物理ダメージ
    case DamageType::Magic:
        return static_cast<int>(std::min(damage / 2.5f, 20.0f));    // 魔法ダメージ
    }

    return 0;
}

//---------------------------------------------------------------------------
//! @brief 防御力を考慮した最終ダメージを計算する
//---------------------------------------------------------------------------
int CalculateFinalDamage(int amount, DamageType damage_type, const PieceStatus& status)
{
    float final_damage = amount;    // 最終ダメージ計算用変数

    // ダメージタイプごとに防御力を考慮して最終ダメージを計算
    if(damage_type == DamageType::Physical) {
        int   armor      = status.GetPhysicalDefense();
        float multiplier = 1.0f - (0.052f * armor) / (0.9f + 0.048f * std::abs(armor));
        final_damage     = amount * multiplier;
    }
    else if(damage_type == DamageType::Magic) {
        float resistance = status.GetMagicalDefense();    // 例: 0.15 = 15%
        float multiplier = 1.0f - resistance;
        final_damage     = amount * multiplier;
    }

    // 最低ダメージは1に設定
    if(final_damage < 1.0)
        final_damage = 1.0;

    // 整数に変換して返す
    return static_cast<int>(final_damage);
}
