//---------------------------------------------------------------------------
//!	@file	TitleScene.h
//! @brief	オートチェスのタイトルシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <Game/AutoChess/system/Timer.h>
#include <Game/AutoChess/Context/GameContext.h>
#include <TsukinoDIContainer/TsukinoDIContainer.hpp>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
#include <Game/AutoChess/Info/MatchInfo.h>
//前方宣言
class TitleScene : public Scene::Base
{
public:
    BP_CLASS_DECL(TitleScene, u8"オートチェスのタイトルシーン")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
private:
    float click_text_rad_ = 0.0f;
};
