//---------------------------------------------------------------------------
//!	@file	ChessPawnSkill.cpp
//! @brief	チェスポーンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessPawnSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChessPawnSkill::ChessPawnSkill()
{
    name_ = "ChessPawnSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChessPawnSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessPawnSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // センサーからピースを取得
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        // 最も近い敵ピースを取得
        if(auto target_enemy = sensor->GetNearestEnemy()) {
            float damage_rate = DAMAGE_RATES_.at(owner->GetLevel() - 1);    //ダメージ倍率を取得
            int   damage      = static_cast<int>(owner->GetAttackPower() * damage_rate);
            target_enemy->TakeDamage(damage, DamageType::Physical);    //ダメージを与える
            //---------------------------------------------------------
            // エフェクトのオブジェクトを生成
            //---------------------------------------------------------
            float3 pos   = target_enemy->GetTranslate();
            auto   skill = Scene::Object::Create<SkillObjectBase>();
            skill->SetEffect("data/AutoChess/Effect/ChessPawnSkill.efkefc");
            skill->SetEffectPlaySpeed(3.0f);
            skill->SetScaleAxisXYZ(1.0f);
            skill->SetTranslate(pos);
            skill->SetSkillOwner(owner->GetOwner());
        }
    }
}
