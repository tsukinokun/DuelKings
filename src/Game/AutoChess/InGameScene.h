//---------------------------------------------------------------------------
//!	@file	InGameScene.h
//! @brief	オートチェスのインゲームシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
class InGameScene : public Scene::Base
{
public:
    BP_CLASS_DECL(InGameScene, u8"オートチェスのインゲームシーン")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
private:
    //ゲームの状態
    enum class GameState
    {
        Setup,     //!< 設置フェーズ
        Battle,    //!< 戦闘フェーズ
    };

private:
    GameState game_state_       = GameState::Setup;    //現在のゲーム状態、設置フェーズから開始する。
    bool      is_purchase_open_ = true;                //ピース購入画面が開いているかどうか
    int       turn_count_       = 0;                   // 現在のターン数
};
