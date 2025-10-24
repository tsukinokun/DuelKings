//---------------------------------------------------------------------------
//!	@file	PieceFactory.cpp
//! @brief	駒生成クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PieceFactory.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Piece/Piece.h>
#include <Game/AutoChess/Piece/JapaneseChessBishop.h>
#include <Game/AutoChess/Piece/JapaneseChessGold.h>
#include <Game/AutoChess/Piece/JapaneseChessKing.h>
#include <Game/AutoChess/Piece/JapaneseChessKnight.h>
#include <Game/AutoChess/Piece/JapaneseChessLance.h>
#include <Game/AutoChess/Piece/JapaneseChessPawn.h>
#include <Game/AutoChess/Piece/JapaneseChessRook.h>
#include <Game/AutoChess/Piece/JapaneseChessSilver.h>
#include <Game/AutoChess/Piece/ChessPawn.h>
#include <Game/AutoChess/Piece/ChessRook.h>
#include <Game/AutoChess/Piece/ChessKnight.h>
#include <Game/AutoChess/Piece/ChessBishop.h>
#include <Game/AutoChess/Piece/ChessQueen.h>
#include <Game/AutoChess/Piece/ChessKing.h>
//---------------------------------------------------------------------------
//! ピース名を受け取って対応する駒インスタンスを生成する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PieceFactory::CreatePiece(const std::string& type)
{
    if(type == "JapaneseChessBishop") {
        //角行
        return Scene::Object::Create<JapaneseChessBishop>();
    }
    else if(type == "JapaneseChessGold") {
        //金将
        return Scene::Object::Create<JapaneseChessGold>();
    }
    else if(type == "JapaneseChessKing") {
        //王将
        return Scene::Object::Create<JapaneseChessKing>();
    }
    else if(type == "JapaneseChessKnight") {
        //桂馬
        return Scene::Object::Create<JapaneseChessKnight>();
    }
    else if(type == "JapaneseChessLance") {
        //香車
        return Scene::Object::Create<JapaneseChessLance>();
    }
    else if(type == "JapaneseChessPawn") {
        //歩兵
        return Scene::Object::Create<JapaneseChessPawn>();
    }
    else if(type == "JapaneseChessRook") {
        //飛車
        return Scene::Object::Create<JapaneseChessRook>();
    }
    else if(type == "JapaneseChessSilver") {
        //銀将
        return Scene::Object::Create<JapaneseChessSilver>();
    }
    else if(type == "ChessPawn") {
        //ポーン
        return Scene::Object::Create<ChessPawn>();
    }
    else if(type == "ChessRook") {
        return Scene::Object::Create<ChessRook>();
    }
    else if(type == "ChessKnight") {
        return Scene::Object::Create<ChessKnight>();
    }
    else if(type == "ChessBishop") {
        return Scene::Object::Create<ChessBishop>();
    }
    else if(type == "ChessQueen") {
        return Scene::Object::Create<ChessQueen>();
    }
    else if(type == "ChessKing") {
        return Scene::Object::Create<ChessKing>();
    }
    else {
        return nullptr;    // 未知のユニット
    }
}
