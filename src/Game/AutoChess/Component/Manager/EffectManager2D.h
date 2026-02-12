//---------------------------------------------------------------------------
//!	@file	EffectManager2D.h
//! @brief	2Dエフェクト管理クラスの定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
USING_PTR(EffectManager2D);
class EffectManager2D : public Component
{
public:
    BP_COMPONENT_DECL(EffectManager2D, u8"2Dエフェクト管理クラス");

    //---------------------------------------------------------------------------
    // Create内で呼ばれる初期化関数
    //! @param event_bus [in] イベントバスのポインタ
    //---------------------------------------------------------------------------
    void Construct(ObjectPtr owner, const std::shared_ptr<TsukinoEventBus::EventBus> event_bus);

    virtual void Init() override;    //!< 初期化
private:
    std::weak_ptr<TsukinoEventBus::EventBus>         event_bus_;        //!< イベントバスのポインタ
    std::vector<TsukinoEventBus::SubscriptionHandle> event_handles_;    //イベントハンドル群
};

CEREAL_REGISTER_TYPE(EffectManager2D)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EffectManager2D)
