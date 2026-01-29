//---------------------------------------------------------------------------
//!	@file	MakrukMetSkill.cpp
//! @brief	メットのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/MakrukMetSkill.h>
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
MakrukMetSkill::MakrukMetSkill()
{
    name_ = "MakrukMetSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void MakrukMetSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void MakrukMetSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // センサーから敵を取得
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        // 最も近い敵を取得
        if(auto target = sensor->GetNearestEnemy()) {
            //---------------------------------------------------------
            // ターゲットにデバフをかける
            //---------------------------------------------------------
            PieceStatusModifier modifier;                                                    // デバフ情報
            modifier.RemovePhysicalDefense(PHYSICAL_DOWN_VALUES_[owner->GetLevel() - 1]);    // 物理防御力を下げる
            target->AddComponent<ModifierStatus>(modifier, EFFECT_DURATION_);                // 効果時間を設定
            //---------------------------------------------------------
            // エフェクトを生成
            //---------------------------------------------------------
            float3 pos   = target->GetTranslate();
            auto   skill = Scene::Object::Create<SkillObjectBase>();
            skill->SetEffect("data/AutoChess/Effect/MakrukMetSkill.efkefc");
            skill->SetEffectPlaySpeed(0.5f);
            skill->SetScaleAxisXYZ(1.0f);
            skill->SetTranslate(pos);
            skill->SetSkillOwner(owner->GetOwner());
        }
    }
}
