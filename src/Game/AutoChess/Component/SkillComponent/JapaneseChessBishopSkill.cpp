//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishopSkill.cpp
//! @brief	香車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessBishopSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/JapaneseChessBishopSkillObject.h>
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/PieceSensor.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessBishopSkill::JapaneseChessBishopSkill()
{
    name_ = "JapaneseChessBishopSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessBishopSkill::Init()
{
    __super::Init();
    mp_ = 90;    //初期MPを設定
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessBishopSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // エフェクトのオブジェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<JapaneseChessBishopSkillObject>();
    skill->SetSkillOwner(owner->GetOwner());
    skill->SetTranslate(pos);
    skill->SetAttackPower(DAMAGE_VALUES_[owner->GetLevel() - 1]);    //ダメージ量を設定);
    //---------------------------------------------------------
    // センサーから最も近い敵ピースの方向へ移動ベクトルを設定
    //---------------------------------------------------------
    if(auto sensor = owner->GetComponent<PieceSensor>()) {
        if(auto target_enemy = sensor->GetNearestEnemy()) {
            float3 vec = target_enemy->GetTranslate() - owner->GetTranslate();
            vec.y      = 0.0f;
            vec        = normalize(vec);    // 水平方向の単位ベクトルに変換
            skill->SetMoveVector(vec * 5.0f);
        }
    }
}
