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

class JapaneseChessBishopSkillObject : public SkillObjectBase
{
public:
    BP_OBJECT_DECL(JapaneseChessBishopSkillObject, u8"角行のスキルのオブジェクト")

    //@
    bool Init() override;    //!< 初期化

private:
    float timer_ = 0.0f;    // タイマー
};
