//---------------------------------------------------------------------------
//!	@file	ChessQueenSkill.cpp
//! @brief	チェスクイーンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessQueenSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/ChessQueenSkillObject.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChessQueenSkill::ChessQueenSkill()
{
    name_ = "ChessQueenSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChessQueenSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessQueenSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // センサーで敵ピースを取得
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        if(auto target_piece = sensor->GetNearestEnemy()) {
            //---------------------------------------------------------
            // スキルオブジェクトを生成して設定
            //---------------------------------------------------------
            auto skill_object = Scene::Object::Create<ChessQueenSkillObject>();
            skill_object->SetSkillOwner(owner->GetOwner());
            skill_object->SetTranslate(target_piece->GetTranslate());
            skill_object->SetDamageRate(DAMAGE_RATE_[owner->GetLevel() - 1]);
        }
    }
}
