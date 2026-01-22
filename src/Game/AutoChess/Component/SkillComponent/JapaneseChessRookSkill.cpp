//---------------------------------------------------------------------------
//!	@file	JapaneseChessRookSkill.cpp
//! @brief	飛車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessRookSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessRookSkill::JapaneseChessRookSkill()
{
    name_ = "JapaneseChessRookSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessRookSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessRookSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査し、敵オブジェクトなら雷を食らわせる
    //---------------------------------------------------------
    auto pieces = Scene::Object::GetArray<Piece>();    // シーン内の全ピースを取得
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        // 所有者エージェントが異なるピースなら
        if(piece->GetOwner() != owner->GetOwner()) {
            //---------------------------------------------------------
            // 雷を食らわせる
            //---------------------------------------------------------
            //確率は50%
            std::random_device              rd;
            std::mt19937                    gen(rd());
            std::uniform_int_distribution<> dis(0, 1);
            //通ったらダメージを与える
            if(dis(gen) == 0) {
                int level_index = owner->GetLevel() - 1;
                piece->TakeDamage(DAMAGE_VALUES_[level_index], DamageType::Magic);    //ダメージを与える
                //残りHPを取得する
                int remaining_hp = piece->GetHP();
                //追加ダメージを計算する
                int additional_damage = static_cast<int>(remaining_hp * ADDITIONAL_DAMAGE_RATE_[level_index]);
                piece->TakeDamage(additional_damage, DamageType::Magic);    //追加の魔法ダメージを与える
            }
            //---------------------------------------------------------
            // エフェクトのオブジェクトを生成
            //---------------------------------------------------------
            float3 pos   = piece->GetTranslate();
            auto   skill = Scene::Object::Create<SkillObjectBase>();
            skill->SetEffect("data/AutoChess/Effect/JapaneseChessRookSkill.efkefc");
            skill->SetSkillOwner(owner->GetOwner());
            skill->SetEffectPlaySpeed(4.0f);
            skill->SetTranslate(pos);
        }
    }
}
