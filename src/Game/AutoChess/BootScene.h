//---------------------------------------------------------------------------
//!	@file	BootScene.h
//! @brief	ブート処理シーンの定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class BootScene : public Scene::Base
{
public:
    BP_CLASS_DECL(BootScene, u8"ブート処理シーン")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
};
