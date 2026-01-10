//---------------------------------------------------------------------------
//!	@file	PiecePurchaseOpenClickEvent.h
//! @brief	ピース購入画面オープンUIをクリックしたときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Synergy/SynergyData.h>
class PiecePurchaseOpenClickEvent : public TsukinoEventBus::BaseEvent
{
public:
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //---------------------------------------------------------------------------
    PiecePurchaseOpenClickEvent() {}
};
