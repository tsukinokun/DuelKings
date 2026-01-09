//---------------------------------------------------------------------------
//!	@file	SynergyModifierData.h
//! @brief	シナジーごとのモディファイアデータをマッピングしたクラス定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include <Game/AutoChess/Synergy/SynergyID.h>
#include <Game/AutoChess/Info/PieceStatusModifier.h>
#include <Game/AutoChess/system/singleton.h>
class SynergyModifierData : public Singleton<SynergyModifierData>
{
private:
    std::unordered_map<SynergyID, std::vector<PieceStatusModifier>> modifier_status_table_;

public:
    //--------------------------------------------------
    // コンストラクタ
    //--------------------------------------------------
    SynergyModifierData();

    //--------------------------------------------------
    // シナジーIDに対応するモディファイアデータを取得
    //! @param id シナジーID
    //! @return 対応するモディファイアデータへの参照
    //--------------------------------------------------
    const std::vector<PieceStatusModifier>& GetModifierData(SynergyID id);
};
