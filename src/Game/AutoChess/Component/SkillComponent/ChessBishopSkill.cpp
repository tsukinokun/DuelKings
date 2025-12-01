//---------------------------------------------------------------------------
//!	@file	ChessBishopSkill.cpp
//! @brief	チェスビショップのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Component/SkillComponent/ChessBishopSkill.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <System/Component/ComponentEffect.h>
//---------------------------------------------------------
//! 初期化
//---------------------------------------------------------
void ChessBishopSkill::Init()
{
    __super::Init();
    mp_ = 100;    // 初期状態でMPを最大にしておく(実験)
}

//---------------------------------------------------------
//! @brief スキル発動処理
//---------------------------------------------------------
void ChessBishopSkill::Activate()
{
    __super::Activate();
    // 3.0f以内の味方ピースのHPを回復する
    auto owner = dynamic_pointer_cast<Piece>(GetOwnerPtr());
    // 回復エフェクトを再生
    const std::string path   = "data/AutoChess/Effect/ChessBishopSkill.efkefc";
    float3            pos    = owner->GetTranslate();
    auto              effect = ComponentEffect::Object::Create(path, pos);
    effect->SetScaleAxisXYZ(2.0f);
    effect->SetTranslate(pos);
    effect->GetComponent<ComponentEffect>()->SetPlaySpeed(0.2f);
    // 味方ピースをループで回復
    auto pieces = Scene::Object::GetArray<Piece>();
    for(auto& piece : pieces) {
        // 所有者が同じなら味方
        if(auto piece_owner = piece->GetOwner()) {
            if(piece_owner == owner->GetOwner()) {
                // 自分自身は回復しない
                if(piece == owner) {
                    continue;
                }
                // 距離を計算
                float distance = length(piece->GetTranslate() - owner->GetTranslate());
                if(distance <= HEAL_RANGE_) {
                    // 回復量を取得
                    int heal_amount = HEAL_AMOUNT_[owner->GetLevel() - 1];
                    // HPを回復
                    piece->Heal(heal_amount);
                }
            }
        }
    }
}
