//---------------------------------------------------------------------------
//!	@file	ChaturangaGaja.cpp
//! @brief	チャトランガのガジャクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "ChaturangaGaja.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/ChaturangaGajaSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool ChaturangaGaja::Init()
{
    __super::Init();
    SetName("ChaturangaGaja");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/ChaturangaGaja.mv1");
    model->SetScaleAxisXYZ(0.005f);
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<ChaturangaGajaSkill>();
    return true;
}
