//---------------------------------------------------------------------------
//!	@file	ChessBoard.h
//! @brief	インゲームシーンのチェスボード
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
USING_PTR(ChessBoard);
class ChessBoard : public Object
{
public:
    BP_OBJECT_DECL(ChessBoard, u8"インゲームシーンのチェスボード")
    //@{
    bool Init() override;      //!< 初期化
    void Update() override;    //!< 更新
    void Draw() override;      //!< 描画
    void Exit() override;      //!< 終了
    void GUI() override;       //!< GUI表示

    //@}
private:
    const int   FILE_MAX_    = 8;    //ファイルの数
    const int   FILE_HALF_   = FILE_MAX_ / 2;
    const int   RANK_MAX_    = 8;    //ランクの数
    const int   RANK_HALF_   = RANK_MAX_ / 2;
    const float SQUARE_SIZE_ = 1.0f;    //マスのxz軸の大きさ
    const float SQUARE_HALF_ = 0.5f;
};
