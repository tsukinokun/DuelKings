//---------------------------------------------------------------------------
//!	@file	MakrukMet.cpp
//! @brief	マークルックのメットクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "MakrukMet.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/MakrukMetSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool MakrukMet::Init()
{
    __super::Init();
    SetName("MakrukMet");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/MakrukMet.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントを追加
    //---------------------------------------------------------------------------------
    AddComponent<MakrukMetSkill>();
    return true;
}
