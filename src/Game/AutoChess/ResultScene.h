//---------------------------------------------------------------------------
//!	@file	ResultScene.h
//! @brief	オートチェスのリザルトシーン
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class ResultScene : public Scene::Base
{
public:
    BP_CLASS_DECL(ResultScene, u8"オートチェスのリザルトシーン")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示
private:
};
