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

    //Onhit時に選択を行うかを返す関数
    //! @retval OnHit時に選択を行うか
    bool IsShouldSelectPiece();

    //Onhit時にドロップを行うかを返す関数
    //! @retval OnHit時にドロップを行うか
    bool IsShouldDropPiece();
    //@}
protected:
    bool should_select_piece_;    //このフレームで、OnHitの選択を行うか否かを保持する変数
    bool should_drop_piece_;      //このフレームで、OnHitのドロップを行うか否かを保持する変数
};
