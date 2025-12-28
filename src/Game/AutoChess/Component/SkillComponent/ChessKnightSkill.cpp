//---------------------------------------------------------------------------
//!	@file	ChessKnightSkill.cpp
//! @brief	チェスナイトのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessKnightSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <random>
//---------------------------------------------------------
//! @brief コンストラクタ
//---------------------------------------------------------
ChessKnightSkill::ChessKnightSkill()
{
    name_ = "ChessKnightSkill";
}

//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void ChessKnightSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessKnightSkill::Activate()
{
    // スキル発動処理の基底クラス呼び出し
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査
    //---------------------------------------------------------
    auto                                pieces = Scene::Object::GetArray<Piece>();    // シーン内のピースを取得
    std::vector<std::shared_ptr<Piece>> ally_pieces;
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        //自身と同じチームのピースをベクターに格納
        if(piece->GetOwner() == owner->GetOwner()) {
            ally_pieces.push_back(piece);
        }
    }
    //---------------------------------------------------------
    // ランダムに味方ピースを選択
    //---------------------------------------------------------
    if(!ally_pieces.empty()) {
        std::random_device              rd;
        std::mt19937                    gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(ally_pieces.size() - 1));
        auto                            selected_ally = ally_pieces[dis(gen)];
        //---------------------------------------------------------
        // 選択した味方ピースのHPを回復する
        //---------------------------------------------------------
        int level_index = owner->GetLevel() - 1;           //レベルインデックスを取得
        selected_ally->Heal(CURE_AMOUNT_[level_index]);    //回復
        //---------------------------------------------------------
        // その味方に近い敵にダメージを与える
        //---------------------------------------------------------
        int damage = DAMAGE_VALUES_[level_index];    //ダメージ量を取得
        for(auto& piece : pieces) {
            //センサーがついていないピースは無視
            if(!piece->GetComponent<PieceSensor>()) {
                continue;
            }
            //自身と別のチームのピースを敵ピースとしてダメージを与える
            if(piece->GetOwner() != owner->GetOwner()) {
                float distance = length(piece->GetTranslate() - selected_ally->GetTranslate());
                // 効果範囲内ならダメージを与える
                if(distance <= EFFECT_RADIUS_) {
                    piece->TakeDamage(damage, DamageType::Fixed);    //固定ダメージを与える
                }
            }
        }
        //---------------------------------------------------------
        // エフェクトを出す
        //---------------------------------------------------------
        float3 pos = selected_ally->GetTranslate();
        //回復エフェクト
        auto skill = Scene::Object::Create<SkillObjectBase>();
        skill->SetEffect("data/AutoChess/Effect/ChessKnightSkillCure.efkefc");
        skill->SetEffectPlaySpeed(3.0f);
        skill->SetScaleAxisXYZ(0.8f);
        skill->SetTranslate(pos);
        skill->SetSkillOwner(owner->GetOwner());
        //攻撃エフェクト
        auto skill2 = Scene::Object::Create<SkillObjectBase>();
        skill2->SetEffect("data/AutoChess/Effect/ChessKnightSkillAttack.efkefc");
        skill2->SetEffectPlaySpeed(3.0f);
        skill2->SetScaleAxisXYZ(0.5f);
        skill2->SetTranslate(pos);
        skill2->SetSkillOwner(owner->GetOwner());
    }
}
