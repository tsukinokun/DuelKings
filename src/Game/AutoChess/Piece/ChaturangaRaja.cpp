//---------------------------------------------------------------------------
//!	@file	ChaturangaRaja.cpp
//! @brief	チャトランガのラージャクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaRaja.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChaturangaRajaSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaRaja::Init()
{
    __super::Init();
    SetName("ChaturangaRaja");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaRaja.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントを追加
    //---------------------------------------------------------------------------------
    AddComponent<ChaturangaRajaSkill>();
    return true;
}
