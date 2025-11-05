//---------------------------------------------------------------------------
//!	@file	ChaturangaAsva.h
//! @brief	チャトランガのアスヴァ(馬)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaAsva);
class ChaturangaAsva : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaAsva, u8"チャトランガのアスヴァクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
