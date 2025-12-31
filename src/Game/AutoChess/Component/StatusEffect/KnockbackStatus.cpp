//---------------------------------------------------------------------------
//!	@file	KnockbackStatus.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/StatusEffect/KnockbackStatus.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void KnockbackStatus::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto knockback_proc = [this]() {
        auto owner = GetOwner();
        //---------------------------------------------------------
        // ノックバック処理
        //---------------------------------------------------------
        owner->AddTranslate(knockback_vector_, false, true);
    };
    SetProc("knockback_proc", knockback_proc, ProcTiming::PreUpdate, ProcPriority::NORMAL);
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void KnockbackStatus::Construct(ObjectPtr owner, float3 knockback_vector, float effect_duration)
{
    __super::Construct(owner);
    knockback_vector_ = knockback_vector;
    effect_duration_  = effect_duration;
    effect_timer_     = 0.0f;
}
