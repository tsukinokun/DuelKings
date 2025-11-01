//---------------------------------------------------------------------------
//!	@file	MakrukMet.cpp
//! @brief	マークルックのメットクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukMet.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukMet::Init()
{
    __super::Init();
    SetName("MakrukMet");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Piece/MakrukMet.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
