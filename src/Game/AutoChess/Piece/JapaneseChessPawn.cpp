//---------------------------------------------------------------------------
//!	@file	JapaneseChessPawn.cpp
//! @brief	将棋の歩兵クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessPawn.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessPawn::Init()
{
    __super::Init();
    SetName("JapaneseChessPawn");
    /*auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/King.mv1");
    model->SetScaleAxisXYZ(0.005f);*/
    return true;
}
