//---------------------------------------------------------------------------
//!	@file	JapaneseChessRook.h
//! @brief	将棋の飛車クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessRook);
class JapaneseChessRook : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessRook, u8"将棋の飛車クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
