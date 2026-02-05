//---------------------------------------------------------------------------
//!	@file	ChessRookSkill.cpp
//! @brief	チェスナイトのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessRookSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveTrackingStrategy.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveToNearestEnemyStrategy.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <random>
#include <System/Component/ComponentCollisionCapsule.h>
//---------------------------------------------------------
//! @brief コンストラクタ
//---------------------------------------------------------
ChessRookSkill::ChessRookSkill()
{
    name_ = "ChessRookSkill";
}

//---------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------
void ChessRookSkill::Init()
{
    __super::Init();
    auto                 owner       = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    std::weak_ptr<Piece> weak_owner  = owner;
    auto                 update_proc = [this, weak_owner]() {
        auto owner = weak_owner.lock();
        if(!owner)
            return;
        //---------------------------------------------------------
        // 追跡中ならば、目的地に到達したことを感知して移動を戻す
        //---------------------------------------------------------
        if(is_tracking_) {
            if(CheckResetMove()) {
                // 移動ストラテジを元に戻す
                if(auto piece_mover = owner->GetComponent<PieceMover>()) {
                    piece_mover->SetMoveStrategy(std::make_unique<MoveToNearestEnemyStrategy>());
                }
                //---------------------------------------------------------
                // オーナーコリジョンをオーバーラップ解除
                //---------------------------------------------------------
                if(auto col_comp = owner->GetComponent<ComponentCollisionCapsule>()) {
                    col_comp->SetOverlapCollisionGroup(static_cast<u32>(0));
                }
                is_tracking_  = false;                                    // 追跡中フラグを下ろす
                attack_count_ = ATTACK_NUM_.at(owner->GetLevel() - 1);    // 攻撃回数を設定
            }
        }
        //---------------------------------------------------------
        // 攻撃回数が残っているならば攻撃を行う
        //---------------------------------------------------------
        if(attack_count_ > 0) {
            interval_timer_ -= GetDeltaTime();
            if(interval_timer_ <= 0) {
                //---------------------------------------------------------
                // 攻撃を行う
                //---------------------------------------------------------
                if(auto target_piece = target_piece_.lock()) {
                    //---------------------------------------------------------
                    // ダメージ量を設定
                    //---------------------------------------------------------
                    float damage = owner->GetAttackPower();                                      // ダメージ量計算
                    target_piece->TakeDamage(static_cast<int>(damage), DamageType::Physical);    // ダメージを与える
                    //---------------------------------------------------------
                    // エフェクトを生成
                    //---------------------------------------------------------
                    float3 pos   = target_piece->GetTranslate();
                    auto   skill = Scene::Object::Create<SkillObjectBase>();    // スキルオブジェクト生成
                    skill->SetEffect("data/AutoChess/Effect/ChessRookSkill.efkefc");
                    skill->SetEffectPlaySpeed(1.0f);
                    skill->SetScaleAxisXYZ(1.0f);
                    skill->SetTranslate(pos);
                    skill->SetSkillOwner(owner->GetOwner());
                }
                attack_count_--;                       //攻撃回数をデクリメント
                interval_timer_ = ATTACK_INTERVAL_;    //カウンタをリセット
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessRookSkill::Activate()
{
    // スキル発動処理の基底クラス呼び出し
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査
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
    // ランダムに敵ピースを選択して、移動ストラテジを変更
    //---------------------------------------------------------
    if(enemy_pieces.size() > 0) {
        // ランダムに敵ピースを選択
        std::random_device              rd;
        std::mt19937                    mt(rd());
        std::uniform_int_distribution<> dist(0, enemy_pieces.size() - 1);
        int                             random_index = dist(mt);
        auto                            target_piece = enemy_pieces[random_index];
        //---------------------------------------------------------
        // 移動ストラテジを追跡に変更
        //---------------------------------------------------------
        if(auto piece_mover = owner->GetComponent<PieceMover>()) {
            std::random_device              rd;
            std::mt19937                    mt(rd());
            std::uniform_int_distribution<> dist(0.0f, 360.0f);
            float                           angle = dist(mt);    // 0〜360度のランダムな角度
            //1.0fの半径でランダムなオフセットを計算
            float3 offset = float3(cosf(D2R(angle)), 0.0f, sinf(D2R(angle)));
            //---------------------------------------------------------
            // オーナーコリジョンをオーバーラップに設定
            //---------------------------------------------------------
            if(auto col_comp = owner->GetComponent<ComponentCollisionCapsule>()) {
                col_comp->SetOverlapCollisionGroup(static_cast<u32>(ComponentCollision::CollisionGroup::ETC));
            }
            piece_mover->SetMoveStrategy(std::make_unique<MoveTrackingStrategy>(target_piece, offset, SPEED_RATE_));    // 追跡ストラテジに変更
            offset_       = offset;                                                                                     // オフセットを保存
            target_piece_ = target_piece;                                                                               // 追跡対象ピースを保存
            is_tracking_  = true;                                                                                       // 追跡中フラグを立てる
        }
    }
}

//---------------------------------------------------------
//! @brief 移動をもとに戻す条件をチェックする関数
//---------------------------------------------------------
bool ChessRookSkill::CheckResetMove()
{
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // そもそもピースムーバーが存在しなければfalseを返す
    //---------------------------------------------------------
    auto piece_mover = owner->GetComponent<PieceMover>();
    if(!piece_mover) {
        return false;
    }
    //---------------------------------------------------------
    // 追跡対象ピースが存在しなければ即座にtrueを返す
    //---------------------------------------------------------
    auto target_piece = target_piece_.lock();
    if(!target_piece) {
        return true;
    }
    //---------------------------------------------------------
    //目的地に到達したかチェック
    //---------------------------------------------------------
    float3 target_position = target_piece->GetTranslate() + offset_;
    float  distance        = length(owner->GetTranslate() - target_position);
    // 自身のスピードを取得
    float speed = owner->GetMoveSpeed();
    //1フレームに移動できる距離を計算
    float frame_move_distance = speed * GetDeltaTime();
    //目的地に到達したらtrueを返す
    if(distance <= frame_move_distance) {
        return true;
    }
    // 到達していなければfalseを返す
    return false;
}
