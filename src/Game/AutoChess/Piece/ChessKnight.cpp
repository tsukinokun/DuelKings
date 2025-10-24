//---------------------------------------------------------------------------
//!	@file	ChessKnight.cpp
//! @brief	チェスのナイトクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessKnight.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessKnight::Init()
{
    __super::Init();
    SetName("ChessKnight");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChessKnight.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
