//---------------------------------------------------------------------------
//!	@file	ChaturangaPadatiSkill.cpp
//! @brief	パダチのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChaturangaPadatiSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChaturangaPadatiSkill::ChaturangaPadatiSkill()
{
    name_ = "ChaturangaPadatiSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChaturangaPadatiSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChaturangaPadatiSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査して敵ピースを格納
    //---------------------------------------------------------
    auto                                pieces = Scene::Object::GetArray<Piece>();
    std::vector<std::shared_ptr<Piece>> enemy_pieces;
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        //自身と違うチームのピースをベクターに格納
        if(piece->GetOwner() != owner->GetOwner()) {
            enemy_pieces.push_back(piece);
        }
    }
    //---------------------------------------------------------
    // ランダムな敵を選択
    //---------------------------------------------------------
    if(enemy_pieces.size() > 0) {
        // ランダムに敵ピースを選択
        std::random_device              rd;
        std::mt19937                    mt(rd());
        std::uniform_int_distribution<> dist(0, enemy_pieces.size() - 1);
        int                             random_index = dist(mt);
        auto                            target_piece = enemy_pieces[random_index];
        //---------------------------------------------------------
        // ダメージ量を設定してダメージを与える
        //---------------------------------------------------------
        float damage = DAMAGE_VALUES_.at(owner->GetLevel() - 1);                  // ダメージ量計算
        target_piece->TakeDamage(static_cast<int>(damage), DamageType::Magic);    // ダメージを与える
        //---------------------------------------------------------
        // 0.1秒間スタンさせる
        //---------------------------------------------------------
        target_piece->AddComponent<StunStatus>(STUN_DURATION_);    // スタンを与える
        //---------------------------------------------------------
        // エフェクトを生成
        //---------------------------------------------------------
        float3 pos   = target_piece->GetTranslate();
        auto   skill = Scene::Object::Create<SkillObjectBase>();    // スキルオブジェクト生成
        {
            skill->SetEffect("data/AutoChess/Effect/ChaturangaPadatiSkill.efkefc");
            skill->SetEffectPlaySpeed(0.5f);
            skill->SetScaleAxisXYZ(1.0f);
            skill->SetTranslate(pos);
            skill->SetSkillOwner(owner->GetOwner());
        }
    }
}
