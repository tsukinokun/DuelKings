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

    //-----------------------------------------------------------
    //Onhit時に選択を行うかを返す関数
    //! @retval OnHit時に選択を行うか
    //-----------------------------------------------------------
    bool IsShouldSelectPiece();

    //-----------------------------------------------------------
    //Onhit時にドロップを行うかを返す関数
    //! @retval OnHit時にドロップを行うか
    //-----------------------------------------------------------
    bool IsShouldDropPiece();

    //-----------------------------------------------------------
    // ピース制限を強制する関数(オーバーライド)
    //-----------------------------------------------------------
    void EnforcePieceLimit() override;

    //-----------------------------------------------------------
    // ピーススタンドとチェスボードの情報を交換する関数
    //! @param piece_stand_index [in] ピーススタンドのインデックス
    //! @param board_file [in] チェスボードのファイル（列）
    //! @param board_rank [in] チェスボードのランク（行）
    //-----------------------------------------------------------
    void SwapPieceStandAndBoardInfo(size_t piece_stand_index, int board_file, int board_rank) override;

    //-----------------------------------------------------------
    // チェスボードの駒をスタンドに移動する関数
    //! @param board_file [in] チェスボードのファイル（列
    //! @param board_rank [in] チェスボードのランク（行）
    //! @note スタンドの左側から空いている場所に移動する
    //-----------------------------------------------------------
    void MoveBoardPieceToStand(int board_file, int board_rank) override;

    //@}
private:
    bool should_select_piece_ = false;    //このフレームで、OnHitの選択を行うか否かを保持する変数
    bool should_drop_piece_   = false;    //このフレームで、OnHitのドロップを行うか否かを保持する変数
};
