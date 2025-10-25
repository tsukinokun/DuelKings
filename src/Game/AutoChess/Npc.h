//---------------------------------------------------------------------------
//!	@file	Npc.h
//! @brief	インゲームシーンのNPC(AI)クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Agent.h"
class ChessBoard;
USING_PTR(Npc);
USING_PTR(PieceStand);
class Npc : public Agent
{
public:
    BP_OBJECT_DECL(Npc, u8"インゲームシーンのNPC(AI)クラス")
    //@{
    bool Init() override;    //!< 初期化

    //@}
private:
};
