//---------------------------------------------------------------------------
//!	@file	Glass.h
//! @brief	インゲームシーンのガラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class Model;    //前方宣言
USING_PTR(Glass);
class Glass : public Object
{
public:
    BP_OBJECT_DECL(Glass, u8"インゲームシーンのガラス")
    //@{
    bool Init() override;    //!< 初期化
    void GUI() override;     //!< GUI表示

    //@}
private:
    std::unique_ptr<Model> model_;
};
