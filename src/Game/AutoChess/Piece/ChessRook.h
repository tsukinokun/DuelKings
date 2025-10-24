//---------------------------------------------------------------------------
//!	@file	ChessRook.h
//! @brief	チェスのルーククラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessRook);
class ChessRook : public Piece
{
public:
    BP_OBJECT_DECL(ChessRook, u8"チェスのルーククラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
