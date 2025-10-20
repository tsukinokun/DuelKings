//---------------------------------------------------------------------------
//!	@file	Agent.h
//! @brief	インゲームシーンのエージェントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class ChessBoard;
class ShopStand;
class Piece;
USING_PTR(Agent);
USING_PTR(PieceStand);
class Agent : public Object
{
public:
    BP_OBJECT_DECL(Agent, u8"インゲームシーンのエージェントクラス")
    //@{
    bool Init() override;    //!< 初期化

    //Onhit時に選択を行うかを返す関数
    //! @retval OnHit時に選択を行うか
    bool IsShouldSelectPiece();

    //Onhit時にドロップを行うかを返す関数
    //! @retval OnHit時にドロップを行うか
    bool IsShouldDropPiece();

    //経験値を増やす関数
    //! @param exp 増やす経験値
    //! @return 自分自身のshared_ptr
    std::shared_ptr<Agent> AddExp(int exp);

    //経験値量から、レベル(置ける駒数)を判定して返す関数
    //! @retval レベル
    int GetAgentLevel();

    //次のレベルまでに必要な経験値を取得する関数
    //! @retval 次のレベルまでに必要な経験値
    int GetNextLevelExp();

    //置かれているピースの数を取得する関数
    //! @retval 置かれているピースの数
    int GetPlacedPieceNum() const;

    //ショップに並んでいるピースを取得する関数
    //! @retval ショップに並んでいるピース
    std::array<std::weak_ptr<Piece>, 5> GetShopPieces();

    //@}
protected:
    bool                      should_select_piece_ = false;    //このフレームで、OnHitの選択を行うか否かを保持する変数
    bool                      should_drop_piece_   = false;    //このフレームで、OnHitのドロップを行うか否かを保持する変数
    int                       exp_                 = 0;        //エージェントのレベル
    PieceStandWeakPtr         stand_;                          // ピーススタンド
    std::weak_ptr<ChessBoard> board_;                          // チェスボード
    std::weak_ptr<ShopStand>  shop_stand_;                     //ショップスタンド
};
