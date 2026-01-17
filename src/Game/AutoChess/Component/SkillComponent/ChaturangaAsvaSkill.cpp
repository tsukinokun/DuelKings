//---------------------------------------------------------------------------
//!	@file	ChaturangaAsvaSkill.cpp
//! @brief	アスヴァのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChaturangaAsvaSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
#include <Game/AutoChess/Component/PieceSensor.h>
#include <Game/AutoChess/SkillObject/SkillObjectBase.h>
#include <Game/AutoChess/Component/StatusEffect/StunStatus.h>
#include <Game/AutoChess/Component/PieceMover.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveTrackingStrategy.h>
#include <Game/AutoChess/Component/MoveStrategy/MoveToNearestEnemyStrategy.h>
#include <random>
//---------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------
ChaturangaAsvaSkill::ChaturangaAsvaSkill()
{
    name_ = "ChaturangaAsvaSkill";
}
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChaturangaAsvaSkill::Init()
{
    __super::Init();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // 更新処理の登録
    //---------------------------------------------------------
    auto update_proc = [this, owner]() {
        //---------------------------------------------------------
        // 追跡中ならば、目的地に到達したことを感知して移動ストラテジを戻す
        //---------------------------------------------------------
        if(is_tracking_) {
            if(CheckResetMove()) {
                // 移動ストラテジを元に戻す
                if(auto piece_mover = owner->GetComponent<PieceMover>()) {
                    piece_mover->SetMoveStrategy(std::make_unique<MoveToNearestEnemyStrategy>());
                }
                is_tracking_ = false;    // 追跡中フラグを下ろす
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    //---------------------------------------------------------
    // ヒット時のコールバック登録
    //---------------------------------------------------------
    OnHitComponentFunc = [this](const HitInfo& hit_info) {
        //---------------------------------------------------------
        // 追跡中でなければ何もしない
        //---------------------------------------------------------
        if(!is_tracking_) {
            return;
        }
        //---------------------------------------------------------
        // 追跡中の処理
        //---------------------------------------------------------
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
                skill->SetEffect("data/AutoChess/Effect/ChaturangaAsvaSkillHit.efkefc");
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
void ChaturangaAsvaSkill::Activate()
{
    __super::Activate();
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    //---------------------------------------------------------
    // ピースを走査して最も遠い敵ピースを取得
    //---------------------------------------------------------
    auto                   pieces       = Scene::Object::GetArray<Piece>();    // シーン内のピースを取得
    std::shared_ptr<Piece> target_piece = nullptr;
    float                  max_distance = -1.0f;    //最大距離初期化
    for(auto& piece : pieces) {
        //センサーがついていないピースは無視
        if(!piece->GetComponent<PieceSensor>()) {
            continue;
        }
        //敵チームのピースのみ対象
        if(piece->GetOwner() == owner->GetOwner()) {
            continue;
        }
        //オーナーからの距離を計算
        float distance = length(piece->GetTranslate() - owner->GetTranslate());
        //最も遠いピースをターゲットに設定
        if(distance > max_distance) {
            max_distance = distance;
            target_piece = piece;
        }
    }
    //---------------------------------------------------------
    // トラッキングに切り替え
    //---------------------------------------------------------
    if(target_piece) {
        //移動ストラテジを追跡に変更
        if(auto piece_mover = owner->GetComponent<PieceMover>()) {
            //オフセットは反対側に設定
            float3 direction = normalize(target_piece->GetTranslate() - owner->GetTranslate());
            offset_          = direction;       //オフセットを保存
            is_tracking_     = true;            //追跡中フラグを立てる
            target_piece_    = target_piece;    //ターゲットピースを保存
            hit_pieces_.clear();                //当たったピースリストをクリア
            piece_mover->SetMoveStrategy(std::make_unique<MoveTrackingStrategy>(target_piece, offset_, TRACKING_SPEED_RATE_));
            //---------------------------------------------------------
            // エフェクトのオブジェクトを生成
            //---------------------------------------------------------
            float3 pos   = owner->GetTranslate();
            auto   skill = Scene::Object::Create<SkillObjectBase>();
            skill->SetEffect("data/AutoChess/Effect/ChaturangaAsvaSkillStart.efkefc");
            skill->SetEffectPlaySpeed(1.0f);
            skill->SetScaleAxisXYZ(1.0f);
            skill->SetTranslate(pos);
            skill->SetSkillOwner(owner->GetOwner());
        }
    }
}

//---------------------------------------------------------
//! @brief 移動をもとに戻す条件をチェックする関数
//---------------------------------------------------------
bool ChaturangaAsvaSkill::CheckResetMove()
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
    //相手にコンポーネントがあるなら
    if(target_piece->GetComponent<ComponentTransform>()) {
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
    }
    // 到達していなければfalseを返す
    return false;
}
