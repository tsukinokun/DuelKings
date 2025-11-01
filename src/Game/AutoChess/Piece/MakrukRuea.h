//---------------------------------------------------------------------------
//!	@file	MakrukRuea.h
//! @brief	マークルックのルアクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(MakrukRuea);
class MakrukRuea : public Piece
{
public:
    BP_OBJECT_DECL(MakrukRuea, u8"マークルックのルアクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
