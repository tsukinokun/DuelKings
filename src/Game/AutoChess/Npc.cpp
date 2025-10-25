//---------------------------------------------------------------------------
//!	@file	Npc.cpp
//! @brief	インゲームシーンのNPCクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include "Npc.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Square.h"
#include "ChessBoard.h"
#include "PieceStand.h"
#include "ShopStand.h"
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Npc::Init()
{
    __super::Init();
    SetName("Npc");
    return true;
}
