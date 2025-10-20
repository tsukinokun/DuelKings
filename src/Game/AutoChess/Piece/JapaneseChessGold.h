//---------------------------------------------------------------------------
//!	@file	JapaneseChessGold.h
//! @brief	将棋の金将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessGold);
class JapaneseChessGold : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessGold, u8"将棋の金将クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
