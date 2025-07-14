//---------------------------------------------------------------------------
//!	@file	MouseRay.cpp
//! @brief	インゲームシーンのマウス光線
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(MouseRay);
class MouseRay : public Object
{
public:
    BP_OBJECT_DECL(MouseRay, u8"インゲームシーンのマウス光線")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
};
