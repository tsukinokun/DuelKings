//---------------------------------------------------------------------------
//!	@file	ChaturangaGaja.h
//! @brief	チャトランガのガジャ(象)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaGaja);
class ChaturangaGaja : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaGaja, u8"チャトランガのガジャクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
