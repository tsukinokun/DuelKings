//---------------------------------------------------------------------------
//!	@file	PieceTargetLocker.h
//! @brief	ピースのターゲットロック機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
class Piece;
USING_PTR(PieceTargetLocker);

class PieceTargetLocker : public Component
{
public:
    BP_COMPONENT_DECL(PieceTargetLocker, u8"ピースのターゲットロック機能クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // ターゲットをロックする関数
    //! @param target ロックするターゲットピース
    //! @param duration ロックする時間
    //---------------------------------------------------------
    void LockTarget(Piece* target, float duration);

private:
    float  active_time_   = 0.0f;       //!< ターゲットロックが有効な時間
    Piece* locked_target_ = nullptr;    //!< ロックしているターゲットピース
};

CEREAL_REGISTER_TYPE(PieceTargetLocker)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceTargetLocker)
