//---------------------------------------------------------------------------
//!	@file	JapaneseChessPawnSkill.cpp
//! @brief	チェスビショップのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessPawnSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/PieceAttacker.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessPawnSkill::JapaneseChessPawnSkill()
{
    name_ = "JapaneseChessPawnSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessPawnSkill::Init()
{
    __super::Init();
    auto owner       = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    auto update_proc = [owner, this]() {
        //タイマーが0.0fより大きければ
        if(effect_timer_ > 0.0f) {
            //タイマーを進める
            effect_timer_ -= GetDeltaTime();
            //タイマーが0.0f以下になったら
            if(effect_timer_ <= 0.0f) {
                int piece_level = owner->GetLevel();                                                  //ピースのレベルを取得する
                owner->RemovePhysicalDefenseModifier(PHYSICAL_DEFENSE_BONUS_.at(piece_level - 1));    //物理防御バフを削除
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessPawnSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 挑発オブジェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/JapaneseChessPawnSkill.efkefc");
    skill->SetEffectPlaySpeed(8.0f);
    skill->SetScaleAxisXYZ(2.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
    //---------------------------------------------------------
    // バフをかける
    //---------------------------------------------------------
    int piece_level = owner->GetLevel();    //ピースのレベルを取得する
    int index       = piece_level - 1;
    effect_timer_   = DURATION_OF_EFFECT_.at(index);                         //タイマーをセット
    owner->AddPhysicalDefenseModifier(PHYSICAL_DEFENSE_BONUS_.at(index));    //物理防御バフを追加
    //---------------------------------------------------------
    // 周囲1.0f以内の敵に自分をロックさせる
    //---------------------------------------------------------
    for(auto& piece : Scene::Object::GetArray<Piece>()) {
        //敵であり、かつ自分からの距離が1.0f以内なら
        if(piece->GetOwner() != owner->GetOwner()) {
            float distance = length(piece->GetTranslate() - owner->GetTranslate());
            if(distance <= 1.0f) {
                //攻撃機能コンポーネントを取得
                if(auto attacker = piece->GetComponent<PieceAttacker>()) {
                    //自分をロックさせる
                    attacker->LockTarget(owner, DURATION_OF_EFFECT_.at(index));
                }
            }
        }
    }
}
