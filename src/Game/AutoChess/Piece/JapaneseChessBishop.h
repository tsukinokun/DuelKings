//---------------------------------------------------------------------------
//!	@file	JapaneseChessBishop.h
//! @brief	将棋の角行クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessBishop);
class JapaneseChessBishop : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessBishop, u8"将棋の角行クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
