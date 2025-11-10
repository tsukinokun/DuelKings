//---------------------------------------------------------------------------
//!	@file	SynergySystem.h
//! @brief	シナジーのシステム関数群
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <Game/AutoChess/Info/BoardInfo.h>
#include <Game/AutoChess/Piece/PieceData/PieceRepository.h>
#include "Synergy.h"
#include "SynergyRepository.h"
class SynergySystem
{
public:
    //--------------------------------------------------
    // デフォルトコンストラクタ
    //--------------------------------------------------
    SynergySystem();

    //--------------------------------------------------
    // リポジトリのポインタを設定
    //! @param repository シナジーリポジトリへのポインタ
    //--------------------------------------------------
    void SetRepository(const SynergyRepository* synergy_repository, const PieceRepository* piece_repository);

    //--------------------------------------------------
    //  ボードをスキャンしてシナジー情報を更新
    //! @param board_info ボード情報への参照
    //--------------------------------------------------
    void UpdateSynergys(const BoardInfo& board_info);

    //--------------------------------------------------
    // シナジーのベクターを返す関数
    //! @return シナジーのベクター
    //--------------------------------------------------
    std::vector<Synergy> GetSynergys() const;

    //--------------------------------------------------
    // 指定したシナジーIDのシナジー情報を取得する関数
    //! @param synergy_id シナジーID
    //! @retval 指定したシナジーIDのシナジー情報、存在しない場合はnullptr
    //--------------------------------------------------
    const SynergyData* GetSynergyData(SynergyID synergy_id) const;

private:
    //--------------------------------------------------
    // 全シナジー情報をリセット
    //--------------------------------------------------
    void Reset();

private:
    std::vector<Synergy>     synergys_;              //!< シナジーのベクター、発動していなくても一体でもいれば管理する。
    const SynergyRepository* synergy_repository_;    //!< しきい値参照用のマスターデータ
    const PieceRepository*   piece_repository_;      //!< 駒定義参照用のマスターデータ
};
