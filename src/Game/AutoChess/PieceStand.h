//---------------------------------------------------------------------------
//!	@file	PieceStand.h
//! @brief	インゲームシーンのピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Square;    //前方宣言
class Agent;
USING_PTR(Piece);
USING_PTR(PieceStand);
class PieceStand : public Object
{
public:
    BP_OBJECT_DECL(PieceStand, u8"インゲームシーンのピース置き場")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //マスの生成
    void CreateSquare();

    //ピースの初期化
    void PieceInit();

    //オーナーの名前を設定
    //! @param owner [in] オーナー
    void SetOwner(std::weak_ptr<Object> owner);

    //マスのウィークポインタを取得
    //! @retval マスのウィークポインタ
    std::array<std::weak_ptr<Square>, 8> GetSquarePtrArray();
    //@}
private:
    const int                            STAND_SQUARE_MAX_  = 8;    //置場マスの数
    const int                            STAND_SQUARE_HALF_ = STAND_SQUARE_MAX_ / 2;
    std::array<std::weak_ptr<Square>, 8> squares_;    //ピース置き場マスのウィークポインタ
    std::weak_ptr<Object>                owner_;      //オーナーのウィークポインタ
};
