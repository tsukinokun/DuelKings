//---------------------------------------------------------------------------
//! @file   FilterColorCorrection.cpp
//! @brief  色補正フィルター
//---------------------------------------------------------------------------
#include "FilterColorCorrection.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool FilterColorCorrection::Init()
{
    // ワークテクスチャを作成
    work_texture_ = std::make_shared<Texture>(WINDOW_W, WINDOW_H, DXGI_FORMAT_R8G8B8A8_UNORM);

    // シェーダーを作成
    shader_ps_ = std::make_shared<ShaderPs>("data/Shader/ps_color_correction.fx");

    auto drawFunc = [this]() {
        // ワークテクスチャに現在のSDRバックバッファをコピー
        // +============+  COPY  +---------------+
        // | BackBuffer | -----> | work_texture_ |
        // +============+        +---------------+
        CopyToRenderTarget(work_texture_.get(), GetBackBuffer());

        // ピクセルシェーダーを適用しながらバックバッファに書き戻す
        // +============+  COPY  +---------------+
        // | BackBuffer | <----- | work_texture_ |
        // +============+  [PS]  +---------------+
        CopyToRenderTarget(GetBackBuffer(), work_texture_.get(), *shader_ps_);
    };

    // 描画関数を登録
    // ProcPriorityは 0～65535 まで設定可能。0が最も優先度が高い(先に実行される)
    SetProc("draw", drawFunc, ProcTiming::Filter, ProcPriority(40000));

    return Super::Init();
}
