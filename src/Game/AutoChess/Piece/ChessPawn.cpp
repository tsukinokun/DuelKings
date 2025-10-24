//---------------------------------------------------------------------------
//!	@file	ChessPawn.cpp
//! @brief	チェスの歩兵クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChessPawn.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChessPawn::Init()
{
    __super::Init();
    SetName("ChessPawn");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChessPawn.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
