//---------------------------------------------------------------------------
//!	@file	MakrukMa.h
//! @brief	マークルックのマークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukMa);
class MakrukMa : public Piece
{
public:
    BP_OBJECT_DECL(MakrukMa, u8"マークルックのマークラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
