//---------------------------------------------------------------------------
//!	@file	MakrukBia.h
//! @brief	マークルックのビアクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukBia);
class MakrukBia : public Piece
{
public:
    BP_OBJECT_DECL(MakrukBia, u8"マークルックのビアクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
