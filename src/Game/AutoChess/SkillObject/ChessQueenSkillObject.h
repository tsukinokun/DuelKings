//---------------------------------------------------------------------------
//!	@file	ChessQueenSkillObject.h
//! @brief	クイーンのスキルのオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "SkillObjectBase.h"
//前方宣言
USING_PTR(ChessQueenSkillObject);

class ChessQueenSkillObject : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(ChessQueenSkillObject, u8"クイーンのスキルのオブジェクト")

    //@
    //---------------------------------------------------------
    //! 初期化関数
    //---------------------------------------------------------
    bool Init() override;

    //---------------------------------------------------------------------------
    // ヒット時のコールバック関数
    //! @param hit_info [in] ヒット情報
    //---------------------------------------------------------------------------
    void OnHit(const ComponentCollision::HitInfo& hit_info) override;

    //---------------------------------------------------------
    // ダメージ率を設定する関数
    //! @param damege_rate ダメージ率
    //---------------------------------------------------------
    void SetDamageRate(float damege_rate);

private:
    float       timer_           = 0.0f;     // タイマー
    int         attack_counter_  = 0;        // 攻撃フレームがきた回数
    float       damege_rate_     = 0.0f;     // ダメージ率
    bool        is_attack_frame_ = false;    // 攻撃フレームかどうか
    const float DAMAGE_RANGE_    = 3.0f;     //ダメージ範囲
};
