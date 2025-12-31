//---------------------------------------------------------------------------
//!	@file	MakrukMaSkill.cpp
//! @brief	メットのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/MakrukMaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/KnockbackStatus.h>
#include <Game/AutoChess/Component/StatusEffect/ModifierStatus.h>
#include <Game/AutoChess/Info/PieceStatusModifier.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
MakrukMaSkill::MakrukMaSkill()
{
    name_ = "MakrukMaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void MakrukMaSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void MakrukMaSkill::Activate()
{
    __super::Activate();
    auto owner       = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    int  level_index = owner->GetLevel() - 1;    //レベルインデックスを取得
    //---------------------------------------------------------
    // 回復とバフを行う
    //---------------------------------------------------------
    {
        owner->Heal(HEAL_AMOUNT_[level_index]);
        PieceStatusModifier status_modifier;
        status_modifier.AddRateAttackInterval(UP_ATTACK_INTERVL_[level_index] * -1.0f);    //攻撃間隔を速くする
        owner->AddComponent<ModifierStatus>(status_modifier, EFFECT_DURATION_);
    }
    //---------------------------------------------------------
    // ピースを走査
    //---------------------------------------------------------
    auto pieces = Scene::Object::GetArray<Piece>();    // シーン内のピースを取得
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        //敵チームのピースにデバフを付与
        if(piece->GetOwner() != owner->GetOwner()) {
            //自身から一定範囲内にいるピースのみ処理
            float3 distance = piece->GetTranslate() - owner->GetTranslate();
            if(static_cast<float>(length(distance)) < AREA_RADIUS_) {
                continue;
            }
            PieceStatusModifier enemy_status_modifier;
            enemy_status_modifier.RemovePhysicalDefense(PHYSICAL_DOWN_VALUES_[level_index]);    //物理防御を下げる
            piece->AddComponent<ModifierStatus>(enemy_status_modifier, EFFECT_DURATION_);
        }
    }
    //---------------------------------------------------------
    // スキルエフェクトを出す
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/MakrukMaSkill.efkefc");
    skill->SetEffectPlaySpeed(2.0f);
    skill->SetScaleAxisXYZ(1.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
}
