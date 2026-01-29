//---------------------------------------------------------------------------
//!	@file	MakrukKhunSkill.cpp
//! @brief	クンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/MakrukKhunSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/KnockbackStatus.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
MakrukKhunSkill::MakrukKhunSkill()
{
    name_ = "MakrukKhunSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void MakrukKhunSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void MakrukKhunSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査し、敵オブジェクトなら攻撃を食らわせる
    //---------------------------------------------------------
    auto pieces = Scene::Object::GetArray<Piece>();    // シーン内の全ピースを取得
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        // 所有者エージェントが異なるピースなら
        if(piece->GetOwner() != owner->GetOwner()) {
            //---------------------------------------------------------
            // そのピースが効果範囲内にいるか判定
            //---------------------------------------------------------
            float distance = length(piece->GetTranslate() - owner->GetTranslate());
            if(distance > EFFECT_RANGE_) {
                continue;    // 範囲外ならスキップ
            }
            //---------------------------------------------------------
            // スキルコンポーネントを取得
            //---------------------------------------------------------
            if(auto active_skill = piece->GetComponent<ComponentActiveSkill>()) {
                int level_index = owner->GetLevel() - 1;
                //---------------------------------------------------------
                // MPを蒸発させる
                //---------------------------------------------------------
                int mp_removed = REMOVE_MP_VALUES_.at(level_index);
                active_skill->RemoveMP(REMOVE_MP_VALUES_.at(level_index));    //MPを減らす
                // 蒸発させたMPに応じたダメージを与える
                int damage_amount = static_cast<int>(mp_removed * DAMAGE_RATE_.at(level_index));
                piece->TakeDamage(damage_amount, DamageType::Magic);
            }
        }
    }
    //---------------------------------------------------------
    // エフェクトのオブジェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/MakrukKhunSkill.efkefc");
    skill->SetSkillOwner(owner->GetOwner());
    skill->SetEffectPlaySpeed(1.0f);
    skill->SetTranslate(pos);
}
