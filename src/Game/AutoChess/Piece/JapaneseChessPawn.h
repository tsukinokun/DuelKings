//---------------------------------------------------------------------------
//!	@file	JapaneseChessPawn.h
//! @brief	将棋の歩兵クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessPawn);
class JapaneseChessPawn : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessPawn, u8"将棋の歩兵クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
