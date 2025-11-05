//---------------------------------------------------------------------------
//!	@file	ChaturangaMantri.h
//! @brief	チャトランガのマントリ(大臣)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChaturangaMantri);
class ChaturangaMantri : public Piece
{
public:
    BP_OBJECT_DECL(ChaturangaMantri, u8"チャトランガのマントリクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
