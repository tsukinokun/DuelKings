//---------------------------------------------------------------------------
//!	@file	JapaneseChessKing.cpp
//! @brief	将棋の王将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "JapaneseChessKing.h"
#include <Game/AutoChess/system/GameConst.h>
#include <System/Component/ComponentModel.h>
#include <Game/AutoChess/Component/SkillComponent/JapaneseChessKingSkill.h>
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool JapaneseChessKing::Init()
{
    __super::Init();
    SetName("JapaneseChessKing");
    auto model = AddComponent<ComponentModel>("data/AutoChess/Model/Piece/King.mv1");
    model->SetScaleAxisXYZ(0.005f);
    model->SetRotationAxisXYZ(float3(0.0f, 90.0f, 0.0f));
    //---------------------------------------------------------------------------------
    // スキルコンポーネントの追加
    //---------------------------------------------------------------------------------
    AddComponent<JapaneseChessKingSkill>();
    return true;
}

//---------------------------------------------------------------------------------
//!	描画
//---------------------------------------------------------------------------------
void JapaneseChessKing::Draw()
{
    __super::Draw();
}

//---------------------------------------------------------------------------------
//!	終了
//---------------------------------------------------------------------------------
void JapaneseChessKing::Exit()
{
    __super::Exit();
}

//---------------------------------------------------------------------------------
//!	GUI表示
//---------------------------------------------------------------------------------
void JapaneseChessKing::GUI()
{
    __super::GUI();
}
