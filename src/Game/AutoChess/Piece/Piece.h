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
    bool Init() override;    //!< 初期化

    //選択状態の設定
    //! @param [in] is_selected 選択状態
    void SetSelect(bool is_selected);

    //選択状態の取得
    //! @retval 選択状態
    bool IsSelect();
    //@}
protected:
    bool is_selected_ = false;    //!< 選択されているかどうか
};
