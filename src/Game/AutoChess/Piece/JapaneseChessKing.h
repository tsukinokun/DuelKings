//---------------------------------------------------------------------------
//!	@file	JapaneseChessKing.h
//! @brief	将棋の王将クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(JapaneseChessKing);
class JapaneseChessKing : public Piece
{
public:
    BP_OBJECT_DECL(JapaneseChessKing, u8"将棋の王将クラス")
    //@{
    bool Init() override;    //!< 初期化
    void Draw() override;    //!< 描画
    void Exit() override;    //!< 終了
    void GUI() override;     //!< GUI表示
    //@}
};
