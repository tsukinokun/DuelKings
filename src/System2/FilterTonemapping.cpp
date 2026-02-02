//---------------------------------------------------------------------------
//! @file   FilterTonemapping.cpp
//! @brief  トーンマッピングフィルター
//---------------------------------------------------------------------------
#include "FilterTonemapping.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool FilterTonemapping::Init()
{
    // シェーダーを作成
    shader_ps_ = std::make_shared<ShaderPs>("data/Shader/ps_tonemapping.fx");

    {
        auto drawFunc = [this]() {
            // 元の描画先に戻す
            SetRenderTarget(GetBackBuffer(), GetDepthStencil());

            // トーンマッピング適用
            CopyToRenderTarget(GetBackBuffer(), GetHdrBuffer(), *shader_ps_);
        };

        // 描画関数を登録
        // ProcPriorityは 0～65535 まで設定可能。0が最も優先度が高い(先に実行される)
        SetProc("draw", drawFunc, ProcTiming::Filter, ProcPriority(32768));
    }

    return Super::Init();
}
