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
    //テストで自陣に飛車を置いておく
    PieceInfo piece_info;
    piece_info.SetTypeName("ChessRook");
    piece_info.SetOwner(dynamic_pointer_cast<Agent>(shared_from_this()));
    SetBoardInfo(1, 1, piece_info);
    return true;
}
