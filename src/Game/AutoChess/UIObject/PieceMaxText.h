#pragma once
//---------------------------------------------------------------------------
//!	@file	PieceMaxText.h
//! @brief	ピースの最大所持数表示テキスト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include "UIText.h"
//ポインタとして使用するための前方宣言
USING_PTR(PieceMaxText);

class PieceMaxText : public UIText
{
public:
    BP_OBJECT_DECL(PieceMaxText, u8"ピースの最大所持数表示テキスト")

    //@}
    bool Init() override;    //!< 初期化
private:
};
