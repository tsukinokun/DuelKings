//---------------------------------------------------------------------------
//!	@file	ChessKnight.h
//! @brief	チェスのナイトクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessKnight);
class ChessKnight : public Piece
{
public:
    BP_OBJECT_DECL(ChessKnight, u8"チェスのナイトクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
