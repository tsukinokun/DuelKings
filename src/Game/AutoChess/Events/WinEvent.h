//---------------------------------------------------------------------------
//!	@file	WinEvent.h
//! @brief	勝利したときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class WinEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    WinEvent() {}
};
