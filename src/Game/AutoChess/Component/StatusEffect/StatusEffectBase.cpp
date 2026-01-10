//---------------------------------------------------------------------------
//!	@file	StatusEffectBase.cpp
//! @brief	状態効果のベースクラス実装
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/StatusEffect/StatusEffectBase.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void StatusEffectBase::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto remove_timer_proc = [this]() {
        effect_timer_ += GetDeltaTime();    // タイマーを進める
        // 時間を超えたら解除
        if(effect_timer_ > effect_duration_) {
            OnRemove();               // 解除時処理を呼ぶ
            RemoveThisComponent();    // コンポーネントを削除する
        }
    };
    SetProc("remove_timer_proc", remove_timer_proc, ProcTiming::Update, ProcPriority::NORMAL);
}

//---------------------------------------------------------------------------
//! @brief 解除時に呼ばれる関数
//---------------------------------------------------------------------------
void StatusEffectBase::OnRemove()
{
    // デフォルトでは何もしない
}
