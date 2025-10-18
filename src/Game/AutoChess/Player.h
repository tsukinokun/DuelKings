//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Agent.h"
class ChessBoard;
USING_PTR(Player);
USING_PTR(PieceStand);
class Player : public Agent
{
public:
    BP_OBJECT_DECL(Player, u8"インゲームシーンのプレイヤークラス")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    //@}
private:
    PieceStandWeakPtr         stand_;    // プレイヤーのピーススタンド
    std::weak_ptr<ChessBoard> board_;
};
