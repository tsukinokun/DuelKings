//---------------------------------------------------------------------------
//!	@file	StatusEffectBase.h
//! @brief	状態効果のベースクラス定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/Component.h>
USING_PTR(StatusEffectBase);
//---------------------------------------------------------------------------
//! @class  StatusEffectBase
//! @brief  状態効果のベースクラス
//---------------------------------------------------------------------------
class StatusEffectBase : public Component
{
public:
    BP_COMPONENT_DECL(StatusEffectBase, u8"状態効果のベースクラス");

    //---------------------------------------------------------------------------
    // 初期化関数
    //---------------------------------------------------------------------------
    virtual void Init() override;

    //---------------------------------------------------------------------------
    // 解除時に呼ばれる関数
    //! @note デフォルトでは何もしない、継承先でオーバーライドして使用する
    //---------------------------------------------------------------------------
    virtual void OnRemove();

protected:
    float effect_duration_ = 0.0f;    // 効果時間
    float effect_timer_    = 0.0f;    // 効果時間タイマー
};

CEREAL_REGISTER_TYPE(StatusEffectBase)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StatusEffectBase)
