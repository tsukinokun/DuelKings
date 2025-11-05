//---------------------------------------------------------------------------
//!	@file	ChaturangaPadati.h
//! @brief	チャトランガのパダチ(歩兵)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaPadati);
class ChaturangaPadati : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaPadati, u8"チャトランガのパダチクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
