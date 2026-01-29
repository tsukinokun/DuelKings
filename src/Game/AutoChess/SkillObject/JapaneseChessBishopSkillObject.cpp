//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishopSkillObject.cpp
//! @brief	チェスビショップのスキルオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessBishopSkillObject.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessBishopSkillObject::Init()
{
    __super::Init();

    SetName(u8"衝撃波");
    SetEffect(u8"data/AutoChess/Effect/JapaneseChessBishopSkill.efkefc");
    SetEffectPlaySpeed(0.5f);
    //---------------------------------------------------------------------------------
    // コリジョンを追加
    //---------------------------------------------------------------------------------
    auto collision = AddComponent<ComponentCollisionSphere>();
    collision->SetRadius(0.5f);
    collision->SetCollisionGroup(ComponentCollision::CollisionGroup::ETC);
    //---------------------------------------------------------------------------------
    // 更新処理のセット
    //---------------------------------------------------------------------------------
    auto update_proc = [this]() {
        //移動処理
        AddTranslate(move_vector_);
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);

    return true;
}

//---------------------------------------------------------------------------
//! @brief ヒット時のコールバック関数
//---------------------------------------------------------------------------
void JapaneseChessBishopSkillObject::OnHit(const ComponentCollision::HitInfo& hit_info)
{
    __super::OnHit(hit_info);
    // 当たったコリジョンのオーナーを取得
    auto hit_owner = hit_info.hit_collision_->GetOwnerPtr();
    // 当たった相手がピースだったら
    if(auto hit_piece = dynamic_pointer_cast<Piece>(hit_owner)) {
        // ピースのオーナーエージェントを取得
        auto hit_piece_owner_agent = hit_piece->GetOwner();
        // 自分のオーナーエージェントを取得
        if(auto owner_agent = owner_agent_ptr_.lock()) {
            // オーナーエージェントが異なっていたら
            if(hit_piece_owner_agent != owner_agent) {
                // すでに当たっているピースなら処理を抜ける
                for(auto hit_piece_in_list : hit_pieces_) {
                    if(hit_piece_in_list == hit_piece.get()) {
                        return;
                    }
                }
                // そうでなければダメージを与える
                int damage = attack_power_;    //ダメージ量を取得
                hit_piece->TakeDamage(damage, DamageType::Magic);
                hit_pieces_.push_back(hit_piece.get());    //当たったピースをリストに追加
            }
        }
    }
}

//---------------------------------------------------------------------------
//! @brief 移動ベクトルを設定する関数
//---------------------------------------------------------------------------
void JapaneseChessBishopSkillObject::SetMoveVector(const float3& move_vector)
{
    move_vector_ = move_vector;
}

//---------------------------------------------------------------------------
//! @brief 攻撃力を設定する関数
//---------------------------------------------------------------------------
void JapaneseChessBishopSkillObject::SetAttackPower(int power)
{
    attack_power_ = power;
}
