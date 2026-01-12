//---------------------------------------------------------------------------
//!	@file	GoldClickEvent.h
//! @brief	シナジーUIをクリックしたときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class GoldClickEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    GoldClickEvent() {}
};
