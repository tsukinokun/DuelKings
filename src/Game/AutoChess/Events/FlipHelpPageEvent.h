//---------------------------------------------------------------------------
//!	@file	FlipHelpPageEvent.h
//! @brief	ヘルプページをめくったときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class FlipHelpPageEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    FlipHelpPageEvent() {}
};
