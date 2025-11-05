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
#include <Game/AutoChess/Piece/MakrukBia.h>
#include <Game/AutoChess/Piece/MakrukKhon.h>
#include <Game/AutoChess/Piece/MakrukKhun.h>
#include <Game/AutoChess/Piece/MakrukMa.h>
#include <Game/AutoChess/Piece/MakrukMet.h>
#include <Game/AutoChess/Piece/MakrukRuea.h>
#include <Game/AutoChess/Piece/ChaturangaAsva.h>
#include <Game/AutoChess/Piece/ChaturangaRatha.h>
#include <Game/AutoChess/Piece/ChaturangaGaja.h>
#include <Game/AutoChess/Piece/ChaturangaPadati.h>
#include <Game/AutoChess/Piece/ChaturangaMantri.h>
#include <Game/AutoChess/Piece/ChaturangaRaja.h>

//mapに、駒の名前と生成関数を登録
const std::unordered_map<std::string_view, PieceFactory::PieceCreator> PieceFactory::piece_creators_ = {
    {"JapaneseChessBishop", [] { return Scene::Object::Create<JapaneseChessBishop>(); }},
    {  "JapaneseChessGold",   [] { return Scene::Object::Create<JapaneseChessGold>(); }},
    {  "JapaneseChessKing",   [] { return Scene::Object::Create<JapaneseChessKing>(); }},
    {"JapaneseChessKnight", [] { return Scene::Object::Create<JapaneseChessKnight>(); }},
    { "JapaneseChessLance",  [] { return Scene::Object::Create<JapaneseChessLance>(); }},
    {  "JapaneseChessPawn",   [] { return Scene::Object::Create<JapaneseChessPawn>(); }},
    {  "JapaneseChessRook",   [] { return Scene::Object::Create<JapaneseChessRook>(); }},
    {"JapaneseChessSilver", [] { return Scene::Object::Create<JapaneseChessSilver>(); }},
    {          "ChessPawn",           [] { return Scene::Object::Create<ChessPawn>(); }},
    {          "ChessRook",           [] { return Scene::Object::Create<ChessRook>(); }},
    {        "ChessKnight",         [] { return Scene::Object::Create<ChessKnight>(); }},
    {        "ChessBishop",         [] { return Scene::Object::Create<ChessBishop>(); }},
    {         "ChessQueen",          [] { return Scene::Object::Create<ChessQueen>(); }},
    {          "ChessKing",           [] { return Scene::Object::Create<ChessKing>(); }},
    {          "MakrukBia",           [] { return Scene::Object::Create<MakrukBia>(); }},
    {         "MakrukKhon",          [] { return Scene::Object::Create<MakrukKhon>(); }},
    {         "MakrukKhun",          [] { return Scene::Object::Create<MakrukKhun>(); }},
    {           "MakrukMa",            [] { return Scene::Object::Create<MakrukMa>(); }},
    {          "MakrukMet",           [] { return Scene::Object::Create<MakrukMet>(); }},
    {         "MakrukRuea",          [] { return Scene::Object::Create<MakrukRuea>(); }},
    {     "ChaturangaAsva",      [] { return Scene::Object::Create<ChaturangaAsva>(); }},
    {    "ChaturangaRatha",     [] { return Scene::Object::Create<ChaturangaRatha>(); }},
    {     "ChaturangaGaja",      [] { return Scene::Object::Create<ChaturangaGaja>(); }},
    {   "ChaturangaPadati",    [] { return Scene::Object::Create<ChaturangaPadati>(); }},
    {   "ChaturangaMantri",    [] { return Scene::Object::Create<ChaturangaMantri>(); }},
    {     "ChaturangaRaja",      [] { return Scene::Object::Create<ChaturangaRaja>(); }},
};
//---------------------------------------------------------------------------
// ピース名を受け取って対応する駒インスタンスを生成する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PieceFactory::CreatePiece(const std::string_view& type, int piece_level)
{
    auto it = piece_creators_.find(type);
    if(it != piece_creators_.end()) {
        auto piece = it->second();
        piece->SetLevel(piece_level);
        return piece;
    }
    return nullptr;
}
