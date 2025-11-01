//---------------------------------------------------------------------------
//!	@file	MakrukKhon.h
//! @brief	マークルックのコーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukKhon);
class MakrukKhon : public Piece
{
public:
    BP_OBJECT_DECL(MakrukKhon, u8"マークルックのコーンクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
