//---------------------------------------------------------------------------
//!	@file	ChessQueenSkillObject.cpp
//! @brief	チェスクイーンのスキルオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ChessQueenSkillObject.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------------
//! @brief 初期化
//---------------------------------------------------------------------------------
bool ChessQueenSkillObject::Init()
{
    __super::Init();

    SetName(u8"ブラックホール");
    SetEffect(u8"data/AutoChess/Effect/ChessQueenSkill.efkefc", true);    //八秒で消すのでループ再生
    SetEffectPlaySpeed(0.2f);
    SetScaleAxisXYZ(0.6f);
    //---------------------------------------------------------------------------------
    // コリジョンを追加
    //---------------------------------------------------------------------------------
    {
        auto collision_sphere = AddComponent<ComponentCollisionSphere>();
        collision_sphere->SetRadius(DAMAGE_RANGE_);    //範囲を設定
    }

    auto update_proc = [this]() {
        timer_ += GetDeltaTime();    //タイマーを進める
        //---------------------------------------------------------------------------------
        // 攻撃フレームかを判定
        //---------------------------------------------------------------------------------
        //1.0秒に1回攻撃フレーム
        if(timer_ > static_cast<float>(attack_counter_)) {
            is_attack_frame_ = true;
            attack_counter_++;
        }
        else {
            is_attack_frame_ = false;
        }
        //---------------------------------------------------------------------------------
        // リリース処理
        //---------------------------------------------------------------------------------
        if(timer_ >= 8.0f) {
            //8秒経過で自分ごと消去
            Scene::Object::Release(SharedThis());
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    return true;
}

//---------------------------------------------------------------------------
//! @brief ヒット時のコールバック関数
//---------------------------------------------------------------------------
void ChessQueenSkillObject::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    //攻撃フレームでなければ何もしない
    if(!is_attack_frame_) {
        return;
    }
    //---------------------------------------------------------------------------
    // 攻撃フレーム時の処理
    //---------------------------------------------------------------------------
    auto hit_owner = hit_info.hit_collision_->GetOwnerPtr();
    // 当たった相手がピースだったら
    if(auto hit_piece = dynamic_pointer_cast<Piece>(hit_owner)) {
        // ピースのオーナーエージェントを取得
        auto hit_piece_owner_agent = hit_piece->GetOwner();
        // 自分のオーナーエージェントを取得
        if(auto owner_agent = owner_agent_ptr_.lock()) {
            // オーナーエージェントが異なっていたら
            if(hit_piece_owner_agent != owner_agent) {
                //---------------------------------------------------------------------------
                // ダメージを与える
                //---------------------------------------------------------------------------
                int hp_remaining = hit_piece->GetHP();               //残りHPを取得
                int damage       = hp_remaining * damege_rate_;      //ダメージ量を取得
                hit_piece->TakeDamage(damage, DamageType::Fixed);    //固定ダメージを与える
            }
        }
    }
}

//---------------------------------------------------------
//! @brief ダメージ率を設定する関数
//---------------------------------------------------------
void ChessQueenSkillObject::SetDamageRate(float damege_rate)
{
    damege_rate_ = damege_rate;
}
