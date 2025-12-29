//---------------------------------------------------------------------------
//!	@file	ChaturangaAsvaSkill.h
//! @brief	アスヴァのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaAsvaSkill);
class Piece;    // 前方宣言
class ChaturangaAsvaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaAsvaSkill, u8"アスヴァのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaAsvaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    //---------------------------------------------------------
    // 移動をもとに戻す条件をチェックする関数
    //! @retval 戻す条件を満たしているならtrue、満たしていないならfalse
    //---------------------------------------------------------
    bool CheckResetMove();

private:
    const std::array<int, 3> DAMAGE_VALUES_       = {150, 250, 350};             // ダメージ倍率
    const float              STUN_DURATION_       = 1.5f;                        // スタン時間
    const float              TRACKING_SPEED_RATE_ = 2.5f;                        // 追跡速度倍率
    float3                   offset_              = float3(0.0f, 0.0f, 0.0f);    // 追跡オフセット
    bool                     is_tracking_         = false;                       // 追跡中かどうか
    std::weak_ptr<Piece>     target_piece_;                                      // 追跡対象ピース
    std::vector<Piece*>      hit_pieces_;                                        // 当たったピースのリスト
};

CEREAL_REGISTER_TYPE(ChaturangaAsvaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaAsvaSkill)
