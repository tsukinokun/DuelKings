//---------------------------------------------------------------------------
//!	@file	ChaturangaAsva.cpp
//! @brief	チャトランガのアスヴァクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaAsva.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaAsva::Init()
{
    __super::Init();
    SetName("ChaturangaAsva");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChaturangaAsva.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
