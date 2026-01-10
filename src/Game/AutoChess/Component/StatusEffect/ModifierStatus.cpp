//---------------------------------------------------------------------------
//!	@file	ModifierStatus.cpp
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/StatusEffect/ModifierStatus.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void ModifierStatus::Init()
{
    __super::Init();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());    // オーナーはピースであることが前提
    //---------------------------------------------------------------------------
    // オーナーピースにバフ・デバフ効果を反映する
    //---------------------------------------------------------------------------
    owner->AddModifier(status_modifier_);
}

//---------------------------------------------------------------------------
//! @brief Create内で呼ばれる初期化関数
//---------------------------------------------------------------------------
void ModifierStatus::Construct(ObjectPtr owner, const PieceStatusModifier& piece_modifier, float effect_duration)
{
    __super::Construct(owner);
    status_modifier_ = piece_modifier;
    effect_duration_ = effect_duration;
    effect_timer_    = 0.0f;
    //---------------------------------------------------------
    // 同じコンポーネントを許容する
    //---------------------------------------------------------
    SetStatus(Component::StatusBit::SameType, true);
}

//---------------------------------------------------------------------------
//! @brief 解除時に呼ばれる関数
//---------------------------------------------------------------------------
void ModifierStatus::OnRemove()
{
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());    // オーナーはピースであることが前提
    //---------------------------------------------------------------------------
    // オーナーピースにバフ・デバフ効果を反映解除する
    //---------------------------------------------------------------------------
    owner->RemoveModifier(status_modifier_);
}
