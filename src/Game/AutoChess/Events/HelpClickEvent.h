//---------------------------------------------------------------------------
//!	@file	HelpClickEvent.h
//! @brief	ヘルプボタンをクリックしたときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class HelpClickEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    HelpClickEvent() {}
};
