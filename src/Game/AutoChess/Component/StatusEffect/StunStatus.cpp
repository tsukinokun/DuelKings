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
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto update_proc = [this]() {
        stun_timer_ += GetDeltaTime();    // スタンタイマーを進める
        // スタン時間を超えたらスタン解除
        if(stun_timer_ > stun_duration_) {
            RemoveThisComponent();
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void StunStatus::Construct(ObjectPtr owner, float stun_duration)
{
    __super::Construct(owner);
    stun_duration_ = stun_duration;
    stun_timer_    = 0.0f;
}
