//---------------------------------------------------------------------------
//!	@file	ChaturangaMantriSkill.cpp
//! @brief	マントリのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChaturangaMantriSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChaturangaMantriSkill::ChaturangaMantriSkill()
{
    name_ = "ChaturangaMantriSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChaturangaMantriSkill::Init()
{
    __super::Init();
    auto update_proc = [this]() {
        //---------------------------------------------------------
        // 効果時間の更新
        //---------------------------------------------------------
        if(effect_timer_ > 0.0f) {
            float delta_time  = GetDeltaTime();
            effect_timer_    -= delta_time;
            interval_timer_  += delta_time;
            //---------------------------------------------------------
            // 効果間隔を超えたら効果を適用
            //---------------------------------------------------------
            if(interval_timer_ >= EFFECT_INTERVAL_) {
                ApplyEffectToTarget();
                interval_timer_ -= interval_timer_;
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChaturangaMantriSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 効果時間を設定
    //---------------------------------------------------------
    effect_timer_ = EFFECT_DURATIONS_;
    //---------------------------------------------------------
    // このフレームでも攻撃処理を行う
    //---------------------------------------------------------
    ApplyEffectToTarget();
    //---------------------------------------------------------
    // スタートエフェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/ChaturangaMantriSkillStart.efkefc");
    skill->SetEffectPlaySpeed(0.4f);
    skill->SetScaleAxisXYZ(1.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
}

//---------------------------------------------------------
//! @brie 攻撃時の効果処理
//---------------------------------------------------------
void ChaturangaMantriSkill::ApplyEffectToTarget()
{
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 範囲内の敵ピースを取得
    //---------------------------------------------------------
    auto                                pieces = Scene::Object::GetArray<Piece>();    // シーン内のピースを取得
    std::vector<std::shared_ptr<Piece>> target_pieces;
    for(auto& piece : pieces) {
        if(!piece->GetComponent<PieceSensor>()) {
            continue;    //センサーがついていないピースは無視
        }
        // 所有者エージェントが異なるピースのみ対象
        if(piece->GetOwner() != owner->GetOwner()) {
            // 距離を計算
            float distance = length(owner->GetTranslate() - piece->GetTranslate());
            // 射程距離内なら対象に追加
            if(distance <= attack_range_) {
                target_pieces.push_back(piece);
            }
        }
    }
    //---------------------------------------------------------
    // ランダムな敵を選択してダメージを与える
    //---------------------------------------------------------
    if(target_pieces.size() > 0) {
        // ランダムに敵ピースを選択
        std::random_device              rd;
        std::mt19937                    mt(rd());
        std::uniform_int_distribution<> dist(0, target_pieces.size() - 1);
        int                             random_index = dist(mt);
        auto                            target_piece = target_pieces[random_index];
        //---------------------------------------------------------
        // ダメージ量を設定してダメージを与える
        //---------------------------------------------------------
        int damage = DAMAGE_VALUES_[owner->GetLevel() - 1];    //ダメージ量を取得
        target_piece->TakeDamage(damage, DamageType::Magic);
        owner->Heal(damage);    //その分回復する
        //---------------------------------------------------------
        // ヒットエフェクトを生成
        //---------------------------------------------------------
        float3 pos   = target_piece->GetTranslate();
        auto   skill = Scene::Object::Create<SkillObjectBase>();
        skill->SetEffect("data/AutoChess/Effect/ChaturangaMantriSkillHit.efkefc");
        skill->SetEffectPlaySpeed(4.0f);
        skill->SetScaleAxisXYZ(1.0f);
        skill->SetTranslate(pos);
        skill->SetSkillOwner(owner->GetOwner());
    }
}
