//---------------------------------------------------------------------------
//!	@file	ChessPawn.h
//! @brief	チェスのポーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessPawn);
class ChessPawn : public Piece
{
public:
    BP_OBJECT_DECL(ChessPawn, u8"チェスのポーンクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
