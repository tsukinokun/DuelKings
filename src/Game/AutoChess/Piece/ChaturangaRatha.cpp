//---------------------------------------------------------------------------
//!	@file	ChaturangaRatha.cpp
//! @brief	チャトランガのラタクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaRatha.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaRatha::Init()
{
    __super::Init();
    SetName("ChaturangaRatha");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaRatha.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
