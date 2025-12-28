//---------------------------------------------------------------------------
//!	@file	ChessKingSkill.cpp
//! @brief	チェスキングのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessKingSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChessKingSkill::ChessKingSkill()
{
    name_ = "ChessKingSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChessKingSkill::Init()
{
    __super::Init();
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessKingSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査して敵ピースを取得する
    //---------------------------------------------------------
    auto                                pieces = Scene::Object::GetArray<Piece>();    // シーン内のピースを取得
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
    // 敵ピースをランダムに選択してその周囲にダメージとデバフ・スタンを与える
    //---------------------------------------------------------
    std::random_device              rd;
    std::mt19937                    gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(enemy_pieces.size() - 1));
    if(enemy_pieces.size() > 0) {
        // ランダムに敵ピースを選択
        auto target_piece = enemy_pieces.at(dis(gen));
        // レベルに応じたインデックスを取得
        int level_index = owner->GetLevel() - 1;
        // ダメージを計算
        int damage = DAMAGE_VALUE_.at(level_index);
        //ターゲットピースの位置を取得
        float3 target_pos = target_piece->GetTranslate();
        //---------------------------------------------------------
        // 敵ピースを走査して、範囲内ならダメージとスタンを与える
        //---------------------------------------------------------
        for(auto& enemy_piece : enemy_pieces) {
            float3 enemy_pos = enemy_piece->GetTranslate();
            // ターゲットピースからの距離を計算
            float distance = length(target_pos - enemy_pos);
            // 効果範囲内ならダメージとスタンとデバフを与える
            if(distance <= EFFECT_RADIUS_) {
                enemy_piece->TakeDamage(damage, DamageType::Magic);                        //ダメージを与える
                enemy_piece->AddComponent<StunStatus>(STUN_DURATIONS_.at(level_index));    //スタンを与える
            }
        }
        //---------------------------------------------------------
        // エフェクトのオブジェクトを生成
        //---------------------------------------------------------
        auto skill = Scene::Object::Create<SkillObjectBase>();
        skill->SetEffect("data/AutoChess/Effect/ChessKingSkill.efkefc");
        skill->SetEffectPlaySpeed(4.0f);
        skill->SetScaleAxisXYZ(2.0f);
        skill->SetTranslate(target_pos);
        skill->SetSkillOwner(owner->GetOwner());
    }
}
