//---------------------------------------------------------------------------
//!	@file	Agent.cpp
//! @brief	インゲームシーンのエージェントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include <Game/AutoChess/system/GameConst.h>
#include "Square.h"
#include "ChessBoard.h"
#include "PieceStand.h"
#include "ShopStand.h"
//---------------------------------------------------------------------------------
//!	初期化
//---------------------------------------------------------------------------------
bool Agent::Init()
{
    __super::Init();
    SetName("Agent");
    //---------------------------------------------------------------------------------
    //	ピーススタンドを作成
    //---------------------------------------------------------------------------------
    auto piece_stand = Scene::Object::Create<PieceStand>();
    piece_stand->SetOwner(shared_from_this());
    piece_stand->CreateSquare();
    stand_ = piece_stand;
    //---------------------------------------------------------------------------------
    //	ショップスタンドを作成
    //---------------------------------------------------------------------------------
    auto shop_stand = Scene::Object::Create<ShopStand>();
    shop_stand->SetOwner(shared_from_this());
    //---------------------------------------------------------------------------------
    //	チェスボードを作成
    //---------------------------------------------------------------------------------
    auto board = Scene::Object::Create<ChessBoard>();
    board->SetOwner(shared_from_this());
    board->CreateSquare();
    board_ = board;
    return true;
}
//---------------------------------------------------------------------------------
//!	OnHit時に選択を行うかを返す関数
//---------------------------------------------------------------------------------
bool Agent::IsShouldSelectPiece()
{
    return should_select_piece_;
}

//---------------------------------------------------------------------------------
//!	OnHit時にドロップを行うかを返す関数
//---------------------------------------------------------------------------------
bool Agent::IsShouldDropPiece()
{
    return should_drop_piece_;
}
//---------------------------------------------------------------------------------
//!	経験値を増やす関数
//---------------------------------------------------------------------------------
std::shared_ptr<Agent> Agent::AddExp(int exp)
{
    exp_ += exp;
    return dynamic_pointer_cast<Agent>(shared_from_this());
}
//---------------------------------------------------------------------------------
//! 経験値量から、レベル(置ける駒数)を判定して返す関数
//---------------------------------------------------------------------------------
int Agent::GetAgentLevel()
{
    int level     = 0;
    int total_exp = 0;
    for(int i = 0; i < exp_table.size(); ++i) {
        total_exp += exp_table[i];
        if(exp_ < total_exp)
            break;
        ++level;
    }

    return level;
}
//---------------------------------------------------------------------------------
//次のレベルまでに必要な経験値を取得する関数
//---------------------------------------------------------------------------------
int Agent::GetNextLevelExp()
{
    int level     = 0;
    int total_exp = 0;
    for(int i = 0; i < exp_table.size(); ++i) {
        total_exp += exp_table[i];
        if(exp_ < total_exp)
            break;
        ++level;
    }
    if(level < exp_table.size()) {
        return exp_table[level];
    }
    else {
        return 0;    //最大レベルの場合は0を返す
    }
}
//---------------------------------------------------------------------------------
//! 置かれているピースの数を取得する関数
//---------------------------------------------------------------------------------
int Agent::GetPlacedPieceNum() const
{
    int placed_piece_num = 0;    // 置かれているピースの数
    if(auto board = board_.lock()) {
        placed_piece_num = board->GetPieceNumOnSquares();
    }
    return placed_piece_num;
}
//---------------------------------------------------------------------------------
//! ショップに並んでいるピースを取得する関数
//---------------------------------------------------------------------------------
std::array<std::weak_ptr<Piece>, 5> Agent::GetShopPieces()
{
    std::array<std::weak_ptr<Piece>, 5> shop_pieces;
    if(auto shop_stand = shop_stand_.lock()) {
        shop_pieces = shop_stand->GetShopPieces();
    }
    return shop_pieces;
}
