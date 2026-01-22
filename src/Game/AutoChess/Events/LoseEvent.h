//---------------------------------------------------------------------------
//!	@file	LoseEvent.h
//! @brief	敗北したときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class LoseEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //! @param agent_damage_amount エージェントが受けたダメージ量
    //---------------------------------------------------------------------------
    LoseEvent(int agent_damage_amount)
        : agent_damage_amount_(agent_damage_amount)
    {
    }

public:
    int agent_damage_amount_;    //!< エージェントが受けたダメージ量
};
