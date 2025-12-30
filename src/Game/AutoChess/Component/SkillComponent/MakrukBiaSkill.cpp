//---------------------------------------------------------------------------
//!	@file	MakrukBiaSkill.cpp
//! @brief	ビアのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/MakrukBiaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/KnockbackStatus.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
MakrukBiaSkill::MakrukBiaSkill()
{
    name_ = "MakrukBiaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void MakrukBiaSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void MakrukBiaSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // センサーから敵を取得
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        if(auto target = sensor->GetNearestEnemy()) {
            //---------------------------------------------------------
            // ダメージ処理
            //---------------------------------------------------------
            int damage = DAMAGE_VALUES_[owner->GetLevel() - 1];
            target->TakeDamage(damage, DamageType::Magic);
            //---------------------------------------------------------
            // ノックバック処理
            //---------------------------------------------------------
            float3 direction        = normalize(target->GetTranslate() - owner->GetTranslate());
            float3 knockback_vector = direction * KNOCKBACK_STRENGTH_;
            target->AddComponent<KnockbackStatus>(knockback_vector, KNOCKBACK_TIME_);
            //---------------------------------------------------------
            // エフェクトを生成
            //---------------------------------------------------------
            float3 pos   = target->GetTranslate();
            auto   skill = Scene::Object::Create<SkillObjectBase>();
            skill->SetEffect("data/AutoChess/Effect/MakrukBiaSkill.efkefc");
            skill->SetEffectPlaySpeed(2.0f);
            skill->SetScaleAxisXYZ(1.0f);
            skill->SetTranslate(pos);
            skill->SetSkillOwner(owner->GetOwner());
        }
    }
}
