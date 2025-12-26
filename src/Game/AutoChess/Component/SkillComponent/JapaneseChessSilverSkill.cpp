//---------------------------------------------------------------------------
//!	@file	JapaneseChessSilverSkill.cpp
//! @brief	銀将のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessSilverSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessSilverSkill::JapaneseChessSilverSkill()
{
    name_ = "JapaneseChessSilverSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessSilverSkill::Init()
{
    __super::Init();
    mp_ = 90;
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessSilverSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ランダムに敵の駒オブジェクトを取得
    //---------------------------------------------------------
    auto                                pieces = Scene::Object::GetArray<Piece>();    // シーン内の全ピースを取得
    std::vector<std::shared_ptr<Piece>> enemies;                                      // 敵ピースのリスト
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        // 所有者エージェントが異なるピースを敵ピースとしてリストに追加
        if(piece->GetOwner() != owner->GetOwner()) {
            enemies.push_back(piece);
        }
    }
    // 敵ピースが存在する場合
    if(!enemies.empty()) {
        //---------------------------------------------------------
        // ランダムに敵ピースを選択
        //---------------------------------------------------------
        std::random_device              rd;
        std::mt19937                    gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(enemies.size() - 1));
        auto                            enemy = enemies[dis(gen)];    // ランダムに敵ピースを選択
        //---------------------------------------------------------
        // 選択した敵ピースにダメージを与える
        //---------------------------------------------------------
        enemy->TakeDamage(DAMAGE_VALUES_[owner->GetLevel() - 1], DamageType::Magic);    //ダメージを与える
        //---------------------------------------------------------
        // エフェクトのオブジェクトを生成
        //---------------------------------------------------------
        float3 pos   = enemy->GetTranslate();
        auto   skill = Scene::Object::Create<SkillObjectBase>();
        skill->SetEffect("data/AutoChess/Effect/JapaneseChessSilverSkill.efkefc");
        skill->SetEffectPlaySpeed(3.0f);
        skill->SetScaleAxisXYZ(1.0f);
        skill->SetTranslate(pos);
        skill->SetSkillOwner(owner->GetOwner());
    }
}
