//---------------------------------------------------------------------------
//!	@file	JapaneseChessLanceSkill.cpp
//! @brief	香車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessLanceSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/PieceAttacker.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveChargeStrategy.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveToNearestEnemyStrategy.h>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
JapaneseChessLanceSkill::JapaneseChessLanceSkill()
{
    name_ = "JapaneseChessLanceSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void JapaneseChessLanceSkill::Init()
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
                //移動コンポーネントを取得
                if(auto piece_mover = owner->GetComponent<PieceMover>()) {
                    // 移動ストラテジーを通常移動ストラテジーへ変更
                    piece_mover->SetMoveStrategy(std::make_unique<MoveToNearestEnemyStrategy>());
                }
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NONE);
    mp_ = 90;    //初期MPを設定
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void JapaneseChessLanceSkill::Activate()
{
    __super::Activate();
    effect_timer_ = DURATION_OF_EFFECT_;    //タイマーをセット
    auto owner    = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 突進エフェクトのオブジェクトを生成
    //---------------------------------------------------------
    float3 pos   = owner->GetTranslate();
    auto   skill = Scene::Object::Create<SkillObjectBase>();
    skill->SetEffect("data/AutoChess/Effect/JapaneseChessLanceSkill.efkefc");
    skill->SetEffectPlaySpeed(1.0f);
    skill->SetScaleAxisXYZ(1.0f);
    skill->SetTranslate(pos);
    skill->SetSkillOwner(owner->GetOwner());
    //---------------------------------------------------------
    // 移動を突進処理へ変更
    //---------------------------------------------------------
    //センサーを取得
    if(auto piece_senser = owner->GetComponent<PieceSensor>()) {
        //移動コンポーネントを取得
        if(auto piece_mover = owner->GetComponent<PieceMover>()) {
            // オーナーの位置を取得
            float3 owner_pos = owner->GetTranslate();
            // 突進ベクトルを生成
            float3 charge_vector = float3(0.0f, 0.0f, 0.0f);
            //センサーで検出した敵ピースを突進のターゲットに設定
            if(auto target_enemy = piece_senser->GetNearestEnemy()) {
                // 敵ピースの位置を取得
                float3 target_pos = target_enemy->GetTranslate();
                // 突進ベクトルを計算
                charge_vector = target_pos - owner_pos;
            }
            // 移動ストラテジーを突進ストラテジーへ変更
            piece_mover->SetMoveStrategy(std::make_unique<MoveChargeStrategy>(charge_vector));
        }
    }
}
