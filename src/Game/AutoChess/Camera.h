//---------------------------------------------------------------------------
//!	@file	Camera.h
//! @brief	インゲームシーンのカメラ
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include <TsukinoEventBus/TsukinoEventBus.hpp>
USING_PTR(Camera);
class Camera : public Object
{
public:
    BP_OBJECT_DECL(Camera, u8"インゲームシーンのカメラ")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //---------------------------------------------------------------------------
    // 揺れはじめる関数
    //! @param duration [in] 揺れの継続時間(秒)
    //---------------------------------------------------------------------------
    void StartShake(float duration);

    //@}

private:
    float shake_timer = -0.000001f;    //揺れのタイマー、float誤差回避のために0未満で初期化
};
