//---------------------------------------------------------------------------
//!	@file	ChaturangaRaja.h
//! @brief	チャトランガのラージャ(王)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaRaja);
class ChaturangaRaja : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaRaja, u8"チャトランガのラージャクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
