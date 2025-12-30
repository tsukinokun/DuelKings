//---------------------------------------------------------------------------
//!	@file	KnockbackStatus.h
//! @brief	ピースのノックバック状態クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/Component.h>
#include <Game/AutoChess/Component/StatusEffect/StatusEffectBase.h>
USING_PTR(KnockbackStatus);
//---------------------------------------------------------------------------
//! @class  KnockbackStatus
//! @brief  ノックバック状態を管理するコンポーネント
//---------------------------------------------------------------------------
class KnockbackStatus : public StatusEffectBase
{
public:
    BP_COMPONENT_DECL(KnockbackStatus, u8"ピースのノックバック状態クラス");

    //---------------------------------------------------------------------------
    // 初期化関数
    //---------------------------------------------------------------------------
    virtual void Init() override;

    //---------------------------------------------------------------------------
    // Create内で呼ばれる初期化関数
    //! @param owner 所有オブジェクトのポインタ
    //! @param knockback_vector ノックバック方向ベクトル
    //! @param effect_duration ノックバック時間
    //---------------------------------------------------------------------------
    void Construct(ObjectPtr owner, float3 knockback_vector, float effect_duration = 0.0f);

private:
    float3 knockback_vector_ = {0.0f, 0.0f, 0.0f};    // ノックバック方向ベクトル
};

CEREAL_REGISTER_TYPE(KnockbackStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, KnockbackStatus)
