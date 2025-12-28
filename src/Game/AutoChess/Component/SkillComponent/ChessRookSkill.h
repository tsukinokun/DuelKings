//---------------------------------------------------------------------------
//!	@file	ChessRookSkill.h
//! @brief	チェスルークのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessRookSkill);
class Piece;    // 前方宣言
class ChessRookSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessRookSkill, u8"チェスルークのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessRookSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    //---------------------------------------------------------
    // 移動をもとに戻す条件をチェックする関数
    //! @retval 戻す条件を満たしているならtrue、満たしていないならfalse
    //---------------------------------------------------------
    bool CheckResetMove();

private:
    const std::array<int, 3> ATTACK_NUM_  = {2, 3, 4};                   // 攻撃回数配列
    const float              SPEED_RATE_  = 2.0f;                        // 移動速度倍率
    float3                   offset_      = float3(0.0f, 0.0f, 0.0f);    // 追跡オフセット
    bool                     is_tracking_ = false;                       // 追跡中かどうか
    std::weak_ptr<Piece>     target_piece_;                              // 追跡対象ピース
    int                      attack_count_    = 0;                       // 攻撃回数カウント
    float                    interval_timer_  = 0.0f;                    // 攻撃間隔カウンタ
    const float              ATTACK_INTERVAL_ = 0.25f;                   // 攻撃間隔
};

CEREAL_REGISTER_TYPE(ChessRookSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessRookSkill)
