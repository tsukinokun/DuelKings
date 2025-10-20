//---------------------------------------------------------------------------
//!	@file	JapaneseChessSilver.h
//! @brief	将棋の銀将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessSilver);
class JapaneseChessSilver : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessSilver, u8"将棋の銀将クラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
