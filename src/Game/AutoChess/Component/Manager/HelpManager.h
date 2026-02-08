//---------------------------------------------------------------------------
//!	@file	HelpManager.h
//! @brief	ヘルプ管理クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
USING_PTR(HelpManager);
class Piece;    //前方宣言
class HelpManager : public Component
{
public:
    BP_COMPONENT_DECL(HelpManager, u8"ヘルプ管理クラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // Create内で呼ばれる初期化関数
    //! @param event_bus [in] イベントバスのポインタ
    //---------------------------------------------------------------------------
    void Construct(ObjectPtr owner, const std::shared_ptr<TsukinoEventBus::EventBus> event_bus);

    //---------------------------------------------------------------------------
    // 効果音の初期化処理
    //---------------------------------------------------------------------------
    void SoundInit();

private:
    std::weak_ptr<TsukinoEventBus::EventBus>         event_bus_;           //!< イベントバスのポインタ
    int                                              help_num_     = 1;    //!< ヘルプ番号
    int                                              max_help_num_ = 4;    //!< 最大ヘルプ番号
    std::vector<TsukinoEventBus::SubscriptionHandle> event_handles_;       //イベントハンドル群
};

CEREAL_REGISTER_TYPE(HelpManager)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, HelpManager)
