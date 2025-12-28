//---------------------------------------------------------------------------
//!	@file	StunStatus.h
//! @brief	ピースのスタン状態クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Component/Component.h>
USING_PTR(StunStatus);
//---------------------------------------------------------------------------
//! @class  ピースのスタン状態クラス
//! @brief  スタン状態を管理するコンポーネント
//---------------------------------------------------------------------------
class StunStatus : public Component
{
public:
    BP_COMPONENT_DECL(StunStatus, u8"ピースのスタン状態クラス");

    //---------------------------------------------------------------------------
    // 初期化関数
    //---------------------------------------------------------------------------
    virtual void Init() override;

    //---------------------------------------------------------------------------
    // Create内で呼ばれる初期化関数
    //! @param owner 所有オブジェクトのポインタ
    //! @param stun_duration スタン時間
    //---------------------------------------------------------------------------
    void Construct(ObjectPtr owner, float stun_duration = 0.0f);

private:
    float stun_duration_ = 0.0f;    // スタン時間
    float stun_timer_    = 0.0f;    // スタンタイマー
};

CEREAL_REGISTER_TYPE(StunStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, StunStatus)
