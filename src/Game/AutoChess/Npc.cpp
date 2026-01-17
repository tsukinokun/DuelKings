//---------------------------------------------------------------------------
//!	@file	Npc.cpp
//! @brief	インゲームシーンのNPCクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "Agent.h"
#include "Npc.h"
#include <Game/AutoChess/Piece/Piece.h>
#include "Square.h"
#include "ChessBoard.h"
#include "PieceStand.h"
#include "ShopStand.h"
#include <random>
//---------------------------------------------------------------------------------
//! @brief	初期化
//---------------------------------------------------------------------------------
bool Npc::Init()
{
    __super::Init();
    SetName("Npc");
    return true;
}

//---------------------------------------------------------------------------
//! @brief ターンが回ってきたときの処理を行う関数
//---------------------------------------------------------------------------
void Npc::OnTurnStart()
{
    PurchasePieces();    //駒を購入
    PurchaseExp();       //経験値を購入
    PlacePieces();       //駒を配置
}

//---------------------------------------------------------------------------
// ピースのリポジトリを設定する関数
//---------------------------------------------------------------------------
void Npc::SetPieceRepository(const PieceRepository* repo)
{
    piece_repo_ = repo;
}

//---------------------------------------------------------------------------
// 駒の購入関数
//---------------------------------------------------------------------------
void Npc::PurchasePieces()
{
    // レベル(駒を置ける数)が所持駒数より大きいなら、駒を購入
    if(GetAgentLevel() > GetOwnedPieceNum()) {
        // ショップピースを取得
        auto shop_pieces = GetShopPieces();
        //ループ
        for(size_t i = 0; i < shop_pieces.size(); ++i) {
            auto piece = shop_pieces[i];
            //情報があれば
            std::string type_name = piece.GetTypeName();
            if(type_name != "") {
                PieceData piece_data = *piece_repo_->FindByTypeName(type_name);
                //ゴールドを所持していたら購入
                if(piece_data.price_ <= GetGold()) {
                    SpendGold(piece_data.price_);    //ゴールドを減らす
                    AddPieceToStand(piece);          //スタンドに駒を追加
                    break;                           //ループを抜ける
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
//! @brief レベルアップ処理
//---------------------------------------------------------------------------
void Npc::PurchaseExp()
{
    const int expCost = 4;    // 経験値購入に必要なゴールド（例: 4）
    if(GetGold() >= expCost) {
        SpendGold(expCost);    // ゴールドを消費
        AddExp(4);             // 経験値を加算
    }
}

//---------------------------------------------------------------------------
//! @brief 駒を評価する関数
//---------------------------------------------------------------------------
int Npc::EvaluatePiece(const PieceInfo& p)
{
    int score = 0;    //評価値
    return score;
}

//---------------------------------------------------------------------------
//! @brief 駒を配置する関数
//---------------------------------------------------------------------------
void Npc::PlacePieces()
{
    while((GetAgentLevel() > board_info_.GetPieceNumOnSquares()) && stand_info_.GetOwnedPieceNum()) {
        //配置可能なマスを取得
        std::vector<std::array<int, 2>> placeable_squares;    //ファイルとランクの配列のベクター
        auto                            board_info = board_info_.GetPieceInfoArray();
        for(int file = 0; file < 4; ++file) {
            for(int rank = 0; rank < 8; ++rank) {
                if(board_info[file][rank].GetTypeName() == "") {
                    //空いているマスなら追加
                    placeable_squares.push_back({file, rank});
                }
            }
        }
        if(placeable_squares.size() == 0) {
            //配置可能なマスがないなら抜ける
            break;
        }

        for(int i = 0; i < PIECE_STAND_SIZE; i++) {
            PieceInfo& piece_info = stand_info_.GetStandPieceRef(i);    //ピーススタンドのピースを参照する
            //ピース情報があれば
            if(piece_info.GetTypeName() != "") {
                //メルセンヌツイスタでどのボードに置くか抽選
                std::random_device                 rd;
                std::mt19937                       mt(rd());
                std::uniform_int_distribution<int> dist(0, static_cast<int>(placeable_squares.size() - 1));
                int                                random_index = dist(mt);
                //ランダムに選ばれたマスにピースを置く
                int file = placeable_squares[random_index][0];
                int rank = placeable_squares[random_index][1];
                SetBoardInfo(file, rank, piece_info);    //ボードにピースを置く
                //スタンドのピースを空にする
                PieceInfo empty_piece;
                stand_info_.SetPieceAt(i, empty_piece);
                //配置したマスを削除
                placeable_squares.erase(placeable_squares.begin() + random_index);
                break;    //ループを抜ける
            }
        }
    }
}
