//---------------------------------------------------------------------------
//!	@file	Player.h
//! @brief	インゲームシーンのプレイヤークラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(Player);
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
};
