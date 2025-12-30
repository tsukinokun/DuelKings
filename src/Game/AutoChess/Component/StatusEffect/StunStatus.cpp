//---------------------------------------------------------------------------
//!	@file	StunStatus.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void StunStatus::Init()
{
    __super::Init();
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void StunStatus::Construct(ObjectPtr owner, float effect_duration)
{
    __super::Construct(owner);
    effect_duration_ = effect_duration;
    effect_timer_    = 0.0f;
}
