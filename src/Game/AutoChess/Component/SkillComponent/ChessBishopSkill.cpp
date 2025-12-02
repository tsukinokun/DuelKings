//---------------------------------------------------------------------------
//!	@file	ChessBishopSkill.cpp
//! @brief	チェスビショップのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessBishopSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/ChessBishopSkillObject.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChessBishopSkill::Init()
{
    __super::Init();
    SetName<ChessBishopSkill>("ChessBishopSkill");
    mp_ = 100;    // 初期状態でMPを最大にしておく(実験)
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessBishopSkill::Activate()
{
    __super::Activate();
    // 3.0f以内の味方ピースのHPを回復する
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    // 回復オブジェクトを生成
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<ChessBishopSkillObject>();
    skill->SetHealAmount(HEAL_AMOUNT_[owner->GetLevel() - 1]);
    skill->SetScaleAxisXYZ(2.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
}
