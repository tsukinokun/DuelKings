//---------------------------------------------------------------------------
//!	@file	ChessBishopSkillObject.h
//! @brief	ビショップのスキルのオブジェクト
//! @auther 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "SkillObjectBase.h"
//前方宣言
USING_PTR(ChessBishopSkillObject);

class ChessBishopSkillObject : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(ChessBishopSkillObject, u8"ビショップのスキルのオブジェクト")

    //@
    bool Init() override;    //!< 初期化

    //---------------------------------------------------------
    // 回復量を設定する関数
    //! @param heal_amount 回復量
    //---------------------------------------------------------
    void SetHealAmount(float heal_amount);

private:
    float       timer_       = 0.0f;    // タイマー
    float       heal_amount_ = 0.0f;    // 回復量
    const float HEAL_RANGE_  = 3.0f;    //回復範囲
};
