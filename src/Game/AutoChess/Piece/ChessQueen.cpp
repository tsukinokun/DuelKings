//---------------------------------------------------------------------------
//!	@file	ChessQueen.cpp
//! @brief	チェスのクイーンクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessQueen.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessQueen::Init()
{
    __super::Init();
    SetName("ChessQueen");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChessQueen.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
