//---------------------------------------------------------------------------
//! @file   Shadowmap.cpp
//! @brief  シャドウマップ
//---------------------------------------------------------------------------
#include "Shadowmap.h"

//---------------------------------------------------------------------------
//! 初期化
//---------------------------------------------------------------------------
bool Shadowmap::Init()
{
    //----------------------------------------------------------
    // シャドウバッファを作成
    //----------------------------------------------------------
    texture_color_ = std::make_shared<Texture>(RESOLUTION, RESOLUTION, DXGI_FORMAT_R8G8B8A8_UNORM);
    texture_depth_ = std::make_shared<Texture>(RESOLUTION, RESOLUTION, DXGI_FORMAT_D32_FLOAT);

    // 【描画順序】
    // (1) シャドウ描画開始
    //     - 描画先をシャドウバッファに設定 SetRenderTarget
    //     - シャドウバッファの値をクリア ClearDepth
    //     - カメラを光源位置に設定
    //
    // (2) (シャドウ用に)シーンを描画
    //
    // (3) シャドウ描画終了
    //     - カラーバッファにデプスバッファをコピー
    //       (これによってシェーダーでデプス値を参照できるようになる)
    //       但し "DxLibのみ" このコピーが必要。
    //     - カメラを元に戻す
    //     - 描画先を元に戻す
    //     - シャドウデプスバッファをテクスチャーとして設定しておく
    //
    // (4) シーンを描画
    //     - シェーダー内で(2)のデプス値と比較して影判定
    //
    auto begin = [this]() {
        // 1フレーム前に設定していたシャドウデプステクスチャを解除
        SetTexture(16, nullptr);

        // 描画先をシャドウバッファに設定
        // カラーバッファが無効な場合はGPUは内部で「倍速z」の機能を有効にする
        SetRenderTarget(nullptr, texture_depth_.get());

        // シャドウバッファ(Depth)の値をクリア
        ClearDepth(texture_depth_.get(), 1.0f);

        // [DxLib] カメラを光源位置に設定
        DxLib::SetCameraViewMatrix(mat_light_view_);
        DxLib::SetupCamera_ProjectionMatrix(mat_light_proj_);
    };

    auto end = [this]() {
        // カメラを元に戻す
        // ※ComponentCamera内でSetProcで実行されているためここでは処理しない

        // 描画先を元に戻す
        SetRenderTarget(GetHdrBuffer(), GetDepthStencil());

        // シャドウデプスをテクスチャに設定しておく
        // 16番以降はDxLibの影響を受けないため、挿しっぱなしにできる
        SetTexture(16, texture_depth_.get());
    };

    // 描画関数を登録
    // ProcPriorityは 0～65535 まで設定可能。0が最も優先度が高い(先に実行される)
    SetProc("begin", begin, ProcTiming::Shadow, ProcPriority(0));
    SetProc("end", end, ProcTiming::Shadow, ProcPriority(65535));

    return Super::Init();
}

//---------------------------------------------------------------------------
//! 更新後処理
//---------------------------------------------------------------------------
void Shadowmap::PostUpdate()
{
    //シャドウの中心座標をカメラの座標にする
    auto object = Scene::GetCurrentCamera();
    if(!object.expired()) {    //カメラが存在するとき
        auto camera      = object.lock();
        center_position_ = camera->GetPosition();
    }

    //----------------------------------------------------------
    // 光源用のカメラ行列を作成
    //----------------------------------------------------------

    //    float3 center_position_ = float3(0.0f, 0.0f, 0.0f);               //!< シャドウの中心ワールド座標
    //    float3 light_dir_ = normalize(float3(1.0f, 1.0f, 1.0f));    //!< 光源がある方向
    //    float  light_height_ = 512.0f;                                 //!< 光源の高さ

    //----------------------------------------------------------
    // 光源の高さまでのベクトルを求める
    //----------------------------------------------------------
    float3 dir;    // 光源の高さまでのベクトル

    constexpr bool STRICT_HEIGHT_CALC = false;    // 近似計算をするかどうか

    if constexpr(STRICT_HEIGHT_CALC) {    // 高さの位置に正確に配置する計算
        // 光源の方向ベクトルを高さ方向に伸ばす場合の倍率を計算
        float height_scale = light_height_ / light_dir_.y;

        dir = light_dir_ * height_scale;
    }
    else {    // 高さ分だけ伸ばすだけの対応
        dir = light_dir_ * light_height_;
    }

    //----------------------------------------------------------
    // 行列を作成
    //----------------------------------------------------------
    float3 look_at = center_position_;
    float3 eye     = center_position_ + dir;    // 高さに到達した場合の光源位置

    float range  = 100.0f;    // 影を映す範囲 (±range m)
    float near_z = 0.01f;
    float far_z  = light_height_ * 10.0f;    // 十分に伸ばす

    mat_light_view_ = matrix::lookAtLH(eye, look_at);
    mat_light_proj_ = matrix::orthographicOffCenterLH(-range, +range, -range, +range, near_z, far_z);
}
