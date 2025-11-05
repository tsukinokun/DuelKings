//---------------------------------------------------------------------------
//!	@file	ChaturangaRaja.cpp
//! @brief	チャトランガのラージャクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaRaja.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaRaja::Init()
{
    __super::Init();
    SetName("ChaturangaRaja");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/ChaturangaRaja.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
