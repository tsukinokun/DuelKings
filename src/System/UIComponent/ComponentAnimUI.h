#pragma once
#include <System/Scene.h>
#include <System/Component/Component.h>

USING_PTR(ComponentAnimUI);

class ComponentAnimUI : public Component
{
public:
    BP_COMPONENT_DECL(ComponentAnimUI, u8"アニメーションUIコンポーネント");
    //! @{
    //	初期化処理
    void Init() override;

    //ImGui
    void GUI() override;

    //アニメーションステータスの設定
    //! @param gh [in] スプライトのハンドル
    //! @param div_num [in] 分割数
    //! @param ex_rate [in] 拡大率
    //! @param update_frame [in] 何フレームに一回アニメーションの更新を行うか
    //! @retval 自身のポインタ
    std::shared_ptr<ComponentAnimUI> SetAnimStatus(int gh, int div_num, float ex_rate = 1.0f, int update_frame = 5);

private:
    //フレームとsrc_xのリセット、実質のアニメーションリセット
    void Reset();
    //! @}

private:
    int    img_          = -1;        // 画像
    int    src_x_        = 0;         // 画像の描画切り取り位置
    float  ex_rate_      = 1.0f;      // 画像の拡縮率
    float2 size_         = {0, 0};    // 画像全体の大きさ
    float2 div_size_     = {0, 0};    // 画像1つの大きさ
    int    update_frame_ = 5;         // 何フレームに一回src_x(Speite画像の座標上の位置)の更新を行うか
    int    frame_        = 0;         // 再生フレームのカウント
    //--------------------------------------------------------------------
    //! @name Cereal処理
    //--------------------------------------------------------------------
    //@{

    //! @brief セーブ
    // @param arc アーカイバ
    // @param ver バージョン
    CEREAL_SAVELOAD(arc, ver) { arc(cereal::make_nvp("Component", cereal::base_class<Component>(this))); }
};

CEREAL_CLASS_VERSION(ComponentAnimUI, 3);
