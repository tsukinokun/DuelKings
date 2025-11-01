//---------------------------------------------------------------------------
//!	@file	MakrukKhun.h
//! @brief	マークルックのクンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukKhun);
class MakrukKhun : public Piece
{
public:
    BP_OBJECT_DECL(MakrukKhun, u8"マークルックのクンクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
