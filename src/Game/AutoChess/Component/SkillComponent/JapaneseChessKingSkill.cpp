//---------------------------------------------------------------------------
//!	@file	JapaneseChessKingSkill.cpp
//! @brief	王将のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessKingSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessKingSkill::JapaneseChessKingSkill()
{
    name_ = "JapaneseChessKingSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessKingSkill::Init()
{
    __super::Init();
    mp_ = 90;    //初期MPを設定
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessKingSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査
    //---------------------------------------------------------
    auto pieces = Scene::Object::GetArray<Piece>();    // シーン内の全ピースを取得
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        //所有エージェントの距離を測る
        float distance = length(piece->GetTranslate() - owner->GetTranslate());
        // 効果範囲外なら無視
        if(distance > EFFECT_RADIUS_) {
            continue;
        }
        //---------------------------------------------------------
        // スキル効果
        //---------------------------------------------------------
        if(piece->GetOwner() == owner->GetOwner()) {
            // 所有者エージェントが同じピースなら
            // 回復量を取得
            int heal_amount = HEAL_AMOUNT_[owner->GetLevel() - 1];
            // 回復
            piece->Heal(heal_amount);
        }
        else {
            // 所有者エージェントが同じピースなら
            // ダメージ量を取得
            int damage = DAMAGE_VALUES_[owner->GetLevel() - 1];
            // ダメージを与える
            piece->TakeDamage(damage, DamageType::Magic);
        }
    }
    //---------------------------------------------------------
    // スキルオブジェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/JapaneseChessKingSkill.efkefc");
    skill->SetScaleAxisXYZ(0.3f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
}
