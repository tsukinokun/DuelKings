//---------------------------------------------------------------------------
//!	@file	ChessBishopSkillObject.cpp
//! @brief	チェスビショップのスキルオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#include "ChessBishopSkillObject.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <Game/AutoChess/Piece/Piece.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessBishopSkillObject::Init()
{
    __super::Init();

    SetName(u8"ポイズン");
    SetEffect(u8"data/AutoChess/Effect/ChessBishopSkill.efkefc");
    SetEffectPlaySpeed(0.05f);

    auto update_proc = [this]() {
        timer_ += GetDeltaTime();
        //1.5秒ごとに回復処理を行う
        if(timer_ < 1.5f) {
            return;
        }
        timer_ = 0.0f;
        // 味方ピースをループで回復
        auto pieces = Scene::Object::GetArray<Piece>();
        for(auto& piece : pieces) {
            // 所有者が同じなら味方
            if(auto piece_owner = piece->GetOwner()) {
                if(auto this_owner = owner_agent_ptr_.lock()) {
                    if(piece_owner == this_owner) {
                        // 距離を計算
                        float distance = length(piece->GetTranslate() - GetTranslate());
                        if(distance <= HEAL_RANGE_) {
                            // 回復量を取得
                            int heal_amount = heal_amount_;
                            // HPを回復
                            piece->Heal(heal_amount);
                        }
                    }
                }
            }
        }
    };
    SetProc("update_proc", update_proc, ProcTiming::Update, ProcPriority::NORMAL);
    return true;
}

//---------------------------------------------------------
//! @brief 回復量を設定する関数
//---------------------------------------------------------
void ChessBishopSkillObject::SetHealAmount(float heal_amount)
{
    heal_amount_ = heal_amount;
}
