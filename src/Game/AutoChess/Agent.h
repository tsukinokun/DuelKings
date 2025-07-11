//---------------------------------------------------------------------------
//!	@file	Agent.h
//! @brief	インゲームシーンのエージェントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(Agent);
class Agent : public Object
{
public:
    BP_OBJECT_DECL(Agent, u8"インゲームシーンのエージェントクラス")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
};
