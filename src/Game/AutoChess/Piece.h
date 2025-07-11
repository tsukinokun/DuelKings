//---------------------------------------------------------------------------
//!	@file	Piece.h
//! @brief	インゲームシーンのピースクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(Piece);
class Piece : public Object
{
public:
    BP_OBJECT_DECL(Piece, u8"インゲームシーンのピースクラス")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
private:
    float RADIUS_ = 0.4f;
};
