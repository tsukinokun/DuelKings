//---------------------------------------------------------------------------
//!	@file	ChaturangaRatha.h
//! @brief	チャトランガのラタ(戦車)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaRatha);
class ChaturangaRatha : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaRatha, u8"チャトランガのラタクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
