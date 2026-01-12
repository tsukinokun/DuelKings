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
    bool is_open_ = true;    // ピース購入画面が開いたかどうかのフラグ

    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //! @param is_open ピース購入画面が開いたかどうかのフラグ
    //---------------------------------------------------------------------------
    PiecePurchaseOpenClickEvent(bool is_open)
        : is_open_(is_open)
    {
    }
};
