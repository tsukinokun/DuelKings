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
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 更新処理
    //---------------------------------------------------------
    auto update_proc = [owner, this]() {
        //タイマーが0.0fより大きければ
        if(effect_timer_ > 0.0f) {
            //タイマーを進める
            effect_timer_ -= GetDeltaTime();
            //タイマーが0.0f以下になったら
            if(effect_timer_ <= 0.0f) {
                hit_pieces_.clear();    //当たったピースのリストをクリア
                //移動コンポーネントを取得
                if(auto piece_mover = owner->GetComponent<PieceMover>()) {
                    // 移動ストラテジーを通常移動ストラテジーへ変更
                    piece_mover->SetMoveStrategy(std::make_unique<MoveToNearestEnemyStrategy>());
                }
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    //---------------------------------------------------------------------------
    // ラムダ式にヒット時のコールバックを登録
    //---------------------------------------------------------------------------
    OnHitComponentFunc = [this](const HitInfo& hit_info) {
        // 自分のオーナーピースを取得
        auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
        // 当たったコリジョンのオーナーを取得
        auto hit_owner = hit_info.hit_collision_->GetOwnerPtr();
        // 当たった相手がピースだったら
        if(auto hit_piece = dynamic_pointer_cast<Piece>(hit_owner)) {
            // ピースのオーナーエージェントを取得
            auto hit_piece_owner_agent = hit_piece->GetOwner();
            // 自分のオーナーエージェントを取得
            auto owner_agent = owner->GetOwner();
            // オーナーエージェントが異なっていたら
            if(hit_piece_owner_agent != owner_agent) {
                // すでに当たっているピースなら処理を抜ける
                for(auto hit_piece_in_list : hit_pieces_) {
                    if(hit_piece_in_list == hit_piece.get()) {
                        return;
                    }
                }
                // そうでなければダメージを与える
                int damage = DAMAGE_VALUES_[owner->GetLevel() - 1];    //ダメージ量を取得
                hit_piece->TakeDamage(damage, DamageType::Physical);
                hit_pieces_.push_back(hit_piece.get());    //当たったピースをリストに追加
                //---------------------------------------------------------
                // エフェクトのオブジェクトを生成
                //---------------------------------------------------------
                float3 pos   = owner->GetTranslate();
                auto   skill = Scene::Object::Create<SkillObjectBase>();
                skill->SetEffect("data/AutoChess/Effect/JapaneseChessLanceSkill.efkefc");
                skill->SetEffectPlaySpeed(1.0f);
                skill->SetScaleAxisXYZ(1.0f);
                skill->SetTranslate(pos);
                skill->SetSkillOwner(owner->GetOwner());
            }
        }
    };
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
