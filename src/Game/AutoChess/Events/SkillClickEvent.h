//---------------------------------------------------------------------------
//!	@file	SkillClickEvent.h
//! @brief	スキルをクリックしたときのイベント定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Component/SkillComponent/SkillData/SkillData.h>
class SkillClickEvent : public TsukinoEventBus::BaseEvent
{
public:
    SkillData skill_data_;    // スキルデータ
    //---------------------------------------------------------------------------
    //! @brief コンストラクタ
    //! @param skill_data スキルデータ
    //---------------------------------------------------------------------------
    SkillClickEvent(const SkillData& skill_data)
        : skill_data_(skill_data)
    {
    }
};
