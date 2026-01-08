//---------------------------------------------------------------------------
//!	@file	SynergyClickEvent.h
//! @brief	シナジーUIをクリックしたときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class SynergyClickEvent : public TsukinoEventBus::BaseEvent
{
public:
    const SynergyData* synergy_data_;    // シナジーデータ
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //! @param skill_data シナジーデータ
    //---------------------------------------------------------------------------
    SynergyClickEvent(const SynergyData* synergy_data)
        : synergy_data_(synergy_data)
    {
    }
};
