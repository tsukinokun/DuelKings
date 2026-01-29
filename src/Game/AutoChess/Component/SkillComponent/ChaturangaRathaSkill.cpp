//---------------------------------------------------------------------------
//!	@file	ChaturangaRathaSkill.cpp
//! @brief	ラタのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChaturangaRathaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChaturangaRathaSkill::ChaturangaRathaSkill()
{
    name_ = "ChaturangaRathaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChaturangaRathaSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChaturangaRathaSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査して範囲内の敵ピースに効果を適用
    //---------------------------------------------------------
    auto pieces = Scene::Object::GetArray<Piece>();
    for(auto& piece : pieces) {
        if(!piece->GetComponent<PieceSensor>()) {
            continue;    //センサーがついていないピースは無視
        }
        // 所有者エージェントが異なるピースのみ対象
        int level_index = owner->GetLevel() - 1;
        if(piece->GetOwner() != owner->GetOwner()) {
            // 距離を計算
            float distance = length(owner->GetTranslate() - piece->GetTranslate());
            // 射程距離内なら効果を適用
            if(distance <= EFFECT_RANGES_[level_index]) {
                // ダメージを与える
                int damage = DAMAGE_VALUES_[level_index];        //ダメージ量を取得
                piece->TakeDamage(damage, DamageType::Magic);    // ダメージを与える
                // スタン状態にする
                piece->AddComponent<StunStatus>(STUN_DURATIONS_[level_index]);
            }
        }
    }
    //---------------------------------------------------------
    // エフェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/ChaturangaRathaSkill.efkefc");
    skill->SetEffectPlaySpeed(1.0f);
    skill->SetScaleAxisXYZ(1.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
}
