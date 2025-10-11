#pragma once
//---------------------------------------------------------------------------
//!	@file	PiecePurchaseOpenButton.h
//! @brief	ピース購入画面を開くボタンオブジェクト
//! @author	山﨑愛
//---------------------------------------------------------------------------
#include <System/Scene.h>
#include "UIButton.h"
//ポインタとして使用するための前方宣言
USING_PTR(PiecePurchaseOpenButton);

class PiecePurchaseOpenButton : public UIButton
{
public:
    BP_OBJECT_DECL(PiecePurchaseOpenButton, u8"ピース購入画面を開くボタンオブジェクト")

    //@}
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
private:
};
