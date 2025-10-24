//---------------------------------------------------------------------------
//!	@file	ChessKing.cpp
//! @brief	チェスのキングクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessKing.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessKing::Init()
{
    __super::Init();
    SetName("ChessKing");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChessKing.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
