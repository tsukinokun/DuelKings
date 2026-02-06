//---------------------------------------------------------------------------
//!	@file	Logic.h
//! @brief	ゲームロジック置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Info/PieceStatus.h>
#include <Game/AutoChess/system/Result.h>
class Agent;    // 前方宣言
//---------------------------------------------------------------------------
//	ラウンド終了時の所持金計算関数
//! @param agent [in] エージェント
//! @param isWin [in] 勝利したかどうか
//! @retval ラウンド終了時の所持金増加量
//! @note 勝利ボーナス、連勝・連敗ボーナス、利子を含む
//---------------------------------------------------------------------------
int CalculateRoundGold(const std::shared_ptr<Agent>& agent, Result result);

//---------------------------------------------------------------------------
// マナ回復量を計算する
//! @param damage		   [in] 与えたダメージ or 受けたダメージ
//! @param damage_type		   [in] 魔法職かどうか
//! @param is_taken_damage [in] 被ダメージによる回復かどうか
//! @return 回復するMP量
//---------------------------------------------------------------------------
int CalculateMPGain(int damage, DamageType damage_type, bool is_taken_damage);

//---------------------------------------------------------------------------
// 防御力を考慮した最終ダメージを計算する
//! @param amount 元のダメージ量
//! @param damage_type ダメージタイプ (Physical / Magic)
//! @param status ステータス参照 (防御値を取得するため)
//! @return 計算後の最終ダメージ量
//---------------------------------------------------------------------------
int CalculateFinalDamage(int amount, DamageType damage_type, const PieceStatus& status);
