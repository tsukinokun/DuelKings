//---------------------------------------------------------------------------
//!	@file	ChessQueen.h
//! @brief	チェスのクイーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessQueen);
class ChessQueen : public Piece
{
public:
    BP_OBJECT_DECL(ChessQueen, u8"チェスのクイーンクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
