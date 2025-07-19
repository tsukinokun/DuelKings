//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
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
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
    //@}
private:
    PieceStandWeakPtr         stand_;    // プレイヤーのピーススタンド
    std::weak_ptr<ChessBoard> board_;
};
