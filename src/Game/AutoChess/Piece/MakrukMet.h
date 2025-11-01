//---------------------------------------------------------------------------
//!	@file	MakrukMet.h
//! @brief	マークルックのメットクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukMet);
class MakrukMet : public Piece
{
public:
    BP_OBJECT_DECL(MakrukMet, u8"マークルックのメットクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
