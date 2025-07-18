//---------------------------------------------------------------------------
//!	@file	Square.h
//! @brief	インゲームシーンのマス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
//前方宣言
class Agent;
class Piece;
USING_PTR(Square);
class Square : public Object
{
public:
    BP_OBJECT_DECL(Square, u8"インゲームシーンのマス")
    //@{
    bool Init() override;                                               //!< 初期化
    void Update() override;                                             //!< 更新
    void Draw() override;                                               //!< 描画
    void Exit() override;                                               //!< 終了
    void GUI() override;                                                //!< GUI表示
    void OnHit(const ComponentCollision::HitInfo& hitInfo) override;    //!<当たり時処理

    //マウスから出る光線にあたっているかを返す
    //! @retval マウスから出る光線に当たっているかどうか
    bool IsRayHit();

    //オーナーの名前を設定
    //! @param owner [in] オーナー
    void SetOwner(std::weak_ptr<Object> owner);

    //置くピースを設定
    //! @param piece [in] 置くピースのセッタ
    void SetPutPiece(std::weak_ptr<Piece> piece);

    //置いてあるピースを取得
    //! retval 置いてあるピースのウィークポインタ
    std::weak_ptr<Piece> GetPutPiece();
    //@}
private:
    bool                  is_ray_hit_;    //マウスから出る光線に当たっているかを保持する変数
    std::weak_ptr<Piece>  piece_;         // おいてあるピース
    std::weak_ptr<Object> owner_;         //オーナーの名前を保持する変数
};
