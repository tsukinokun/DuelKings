//---------------------------------------------------------------------------
//!	@file	JapaneseChessPawnSkill.cpp
//! @brief	チェスビショップのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessPawnSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
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
    mp_              = 100;    // 初期状態でMPを最大にしておく(実験)
    auto update_proc = [this]() {
        //タイマーが0.0fより小さければ
        if(effect_timer_ < 0.0f) {
            //タイマーを進める
            effect_timer_ -= GetDeltaTime();
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
    int piece_level = owner->GetLevel();                          //ピースのレベルを取得する
    effect_timer_   = DURATION_OF_EFFECT_.at(piece_level - 1);    //タイマーをセット
}
