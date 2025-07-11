//---------------------------------------------------------------------------
//!	@file	PieceStand.h
//! @brief	インゲームシーンのピース置き場
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
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

    //@}
private:
    const int   STAND_SQUARE_MAX_  = 8;    //置場マスの数
    const int   STAND_SQUARE_HALF_ = STAND_SQUARE_MAX_ / 2;
    const float SQUARE_SIZE_       = 1.0f;    //マスのxz軸の大きさ
    const float SQUARE_HALF_       = 0.5f;
};
