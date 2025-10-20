//---------------------------------------------------------------------------
//!	@file	JapaneseChessKnight.h
//! @brief	将棋の桂馬クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessKnight);
class JapaneseChessKnight : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessKnight, u8"将棋の桂馬クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
