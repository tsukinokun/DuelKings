//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Agent.h"
class ChessBoard;
class Piece;
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
    //! @retval 成功した場合 true 、失敗した場合 false
    //! @note スタンドの左側から空いている場所に移動する
    //-----------------------------------------------------------
    bool MoveBoardPieceToStand(int board_file, int board_rank) override;

    //-----------------------------------------------------------
    // 選択されているピースを取得する関数
    //! @retval 選択されているピースのshared_ptr
    //-----------------------------------------------------------
    std::shared_ptr<Piece> GetSelectedPiece() const;

    //-----------------------------------------------------------
    // ピース購入画面が開いているかどうかのフラグへのポインタを設定する関数
    //! @param is_purchase_open [in] ピース購入画面が開いているかどうかのフラグへのポインタ
    //-----------------------------------------------------------
    void SetIsPurchaseOpenFlag(bool* is_purchase_open);

    //@}
private:
    bool                   should_select_piece_ = false;      //このフレームで、OnHitの選択を行うか否かを保持する変数
    bool                   should_drop_piece_   = false;      //このフレームで、OnHitのドロップを行うか否かを保持する変数
    std::shared_ptr<Piece> selected_piece_      = nullptr;    //選択されているピースデータのポインタ
    bool*                  is_purchase_open_    = nullptr;    //ピース購入画面が開いているかどうかのフラグへのポインタ
};
