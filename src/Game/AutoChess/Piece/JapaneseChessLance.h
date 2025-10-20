//---------------------------------------------------------------------------
//!	@file	JapaneseChessLance.h
//! @brief	将棋の王将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessLance);
class JapaneseChessLance : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessLance, u8"将棋の香車クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
