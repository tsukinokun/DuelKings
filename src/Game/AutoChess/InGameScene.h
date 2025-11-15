//---------------------------------------------------------------------------
//!	@file	InGameScene.h
//! @brief	オートチェスのインゲームシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/system/Timer.h>
#include <Game/AutoChess/Context/GameContext.h>
class InGameScene : public Scene::Base
{
private:
    //ゲームの状態
    enum class GameState
    {
        Setup,     //!< 設置フェーズ
        Battle,    //!< 戦闘フェーズ
    };

public:
    BP_CLASS_DECL(InGameScene, u8"オートチェスのインゲームシーン")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

private:
    //----------------------------------------------------------------------
    // フェーズ遷移処理
    //! @param state 遷移したい状態
    //----------------------------------------------------------------------
    void TransitionTo(GameState state);
    //----------------------------------------------------------------------
    // バトルフェーズ開始時に、駒を生成する関数
    //----------------------------------------------------------------------
    void CreatePiecesForBattlePhase();

    //----------------------------------------------------------------------
    // バトルフェーズ終了時に、駒を破棄する関数
    //----------------------------------------------------------------------
    void DestroyPiecesAfterBattlePhase();

    //----------------------------------------------------------------------
    // バトルフェーズの処理
    //----------------------------------------------------------------------
    void UpdateBattlePhase();

    //@}
private:
    Timer       phase_timer_;
    GameContext game_context_;                           //マスターデータなどの管理クラス
    GameState   game_state_       = GameState::Setup;    //現在のゲーム状態、設置フェーズから開始する。
    float       state_timer_      = 0.0f;                // 状態経過時間（秒）
    bool        is_purchase_open_ = true;                //ピース購入画面が開いているかどうか
    bool        has_battle_ended_ = false;               //バトルが終了したかどうか
    int         turn_count_       = 1;                   // 現在のターン数
};
