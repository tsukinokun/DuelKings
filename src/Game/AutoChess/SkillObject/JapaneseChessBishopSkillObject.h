//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishopSkillObject.h
//! @brief	角行のスキルのオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "SkillObjectBase.h"
//前方宣言
USING_PTR(JapaneseChessBishopSkillObject);
class Piece;
class JapaneseChessBishopSkillObject : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(JapaneseChessBishopSkillObject, u8"角行のスキルのオブジェクト")

    //@
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------------------------
    // ヒット時のコールバック関数
    //! @param hit_info [in] ヒット情報
    //---------------------------------------------------------------------------
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    //---------------------------------------------------------------------------
    // 移動ベクトルを設定する関数
    //! @param move_vector [in] 移動ベクトル
    //---------------------------------------------------------------------------
    void SetMoveVector(const float3& move_vector);

    //---------------------------------------------------------------------------
    // 攻撃力を設定する関数
    //! @param power [in] 攻撃力
    //---------------------------------------------------------------------------
    void SetAttackPower(int power);

private:
    float3              move_vector_  = float3{0.0f, 0.0f, 0.0f};    // 移動ベクトル
    int                 attack_power_ = 0;                           // 攻撃力
    std::vector<Piece*> hit_pieces_;                                 //当たったピースのリスト
};
