//---------------------------------------------------------------------------
//!	@file	ChaturangaRajaSkill.cpp
//! @brief	ラジャのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChaturangaRajaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/ChaturangaRajaSkillObject.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChaturangaRajaSkill::ChaturangaRajaSkill()
{
    name_ = "ChaturangaRajaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChaturangaRajaSkill::Init()
{
    __super::Init();
    mp_ = 90;
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChaturangaRajaSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // センサーを取得して、最も近い敵までの方向ベクトルを計算
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        // 最も近い敵を取得
        if(auto target = sensor->GetNearestEnemy()) {
            //---------------------------------------------------------
            // 方向ベクトルを計算
            //---------------------------------------------------------
            float3 direction = target->GetTranslate() - owner->GetTranslate();
            direction.y      = 0.0f;                    //Y成分を0にする
            direction        = normalize(direction);    //正規化
            //レベルインデックスを取得
            int level_index = owner->GetLevel() - 1;
            //---------------------------------------------------------
            // スキルオブジェクトを発生させる
            //---------------------------------------------------------
            float3 pos   = owner->GetTranslate();
            auto   skill = Scene::Object::Create<ChaturangaRajaSkillObject>();
            skill->SetSkillOwner(owner->GetOwner());
            skill->SetTranslate(pos);
            skill->SetAttackPower(DEFAULT_DAMAGE_VALUES_[level_index]);       //ダメージ量を設定;
            skill->SetIncreasePower(INCREASE_DAMAGE_VALUES_[level_index]);    //増加ダメージ量を設定
            float speed_rate = 15.0f;                                         //移動速度倍率を設定
            skill->SetMoveVector(direction * speed_rate);                     //方向ベクトルを設定
        }
    }
}
