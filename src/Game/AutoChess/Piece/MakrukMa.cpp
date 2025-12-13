//---------------------------------------------------------------------------
//!	@file	MakrukMa.cpp
//! @brief	マークルックのマークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukMa.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukMa::Init()
{
    __super::Init();
    SetName("MakrukMa");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/MakrukMa.mv1");
    model->SetScaleAxisXYZ(0.005f);
    return true;
}
