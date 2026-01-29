//---------------------------------------------------------------------------
//!	@file	MakrukRuaSkill.cpp
//! @brief	ルアのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/MakrukRuaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/ModifierStatus.h>
#include <Game/AutoChess/Info/PieceStatusModifier.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
MakrukRuaSkill::MakrukRuaSkill()
{
    name_ = "MakrukRuaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void MakrukRuaSkill::Init()
{
    __super::Init();
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto skill_proc = [this]() {
        // オーナーを取得
        auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
        //---------------------------------------------------------
        // 攻撃処理
        //---------------------------------------------------------
        if(attack_count_ > 0) {
            attack_timer_ += GetDeltaTime();    // タイマーを進める
            // タイマーが間隔を超えたら攻撃
            if(attack_timer_ > EFFECT_INTERVAL_) {
                attack_timer_ = 0.0f;    // タイマーリセット
                //センサーから最も近い敵を取得
                if(auto sensor = owner->GetComponent<PieceSensor>()) {
                    if(auto target_enemy = sensor->GetNearestEnemy()) {
                        int level_index = owner->GetLevel() - 1;
                        //---------------------------------------------------------
                        // target_enemyにダメージを与える
                        //---------------------------------------------------------
                        target_enemy->TakeDamage(DAMAGE_VALUES_.at(level_index), DamageType::Physical);    //ダメージを与える
                        //---------------------------------------------------------
                        // エフェクトを生成
                        //---------------------------------------------------------
                        float3 pos   = target_enemy->GetTranslate();
                        auto   skill = Scene::Object::Create<SkillObjectBase>();
                        skill->SetEffect("data/AutoChess/Effect/MakrukRuaSkill.efkefc");
                        skill->SetEffectPlaySpeed(1.0f);
                        skill->SetScaleAxisXYZ(0.4f);
                        skill->SetTranslate(pos);
                        skill->SetSkillOwner(owner->GetOwner());
                        //---------------------------------------------------------
                        // 攻撃回数を減らす
                        //---------------------------------------------------------
                        attack_count_--;    //攻撃回数を減らす
                    }
                }
            }
        }
    };
    SetProc("skill_proc", skill_proc, ProcTiming::Update, ProcPriority::NORMAL);
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void MakrukRuaSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 攻撃回数を補充(代入)
    //---------------------------------------------------------
    int level_index = owner->GetLevel() - 1;
    attack_count_   = TOTAL_ATTACKS_.at(level_index);
    //---------------------------------------------------------
    // タイマーリセット
    //---------------------------------------------------------
    attack_timer_ = 0.0f;
}
