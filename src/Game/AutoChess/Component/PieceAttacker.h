//---------------------------------------------------------------------------
//!	@file	PieceAttacker.h
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(PieceAttacker);
class Piece;    //前方宣言
class PieceAttacker : public Component
{
public:
    BP_COMPONENT_DECL(PieceAttacker, u8"ピースの攻撃機能クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // ターゲットをロックする関数
    //! @param target ロックするターゲットピース
    //! @param duration ロックする時間
    //---------------------------------------------------------
    void LockTarget(Piece* target, float duration);

private:
    float  attack_timer_  = 0.0f;       //!< 攻撃クールタイムのタイマー
    float  lock_timer_    = 0.0f;       //!< ターゲットロックが有効な時間
    Piece* locked_target_ = nullptr;    //!< ロックしているターゲットピース
};

CEREAL_REGISTER_TYPE(PieceAttacker)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceAttacker)
