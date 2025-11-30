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
