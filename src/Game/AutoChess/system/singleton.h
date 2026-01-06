//---------------------------------------------------------------------------
//! @file   singleton.h
//! @brief  シングルトン
//---------------------------------------------------------------------------
#pragma once
//===========================================================================
//! シングルトンテンプレート
//! public継承することによってシングルトン機能を付与します。
//! インスタンスの重複生成自体を禁止するものではなく、重複生成時にエラー検出します。
//! static関数内にインスタンスを隠蔽する基礎的なシングルトン実装とは異なり、
//! インスタンスの配置場所を選ばない方式です (Game Programming Gems 2)
//===========================================================================
template <typename T>
class Singleton
{
    static inline std::atomic<T*> instance_;    //!< 唯一のインスタンスへのポインタ

public:
    //----------------------------------------------------------
    //! @name   初期化
    //----------------------------------------------------------
    //!@{

    //! コンストラクタ
    Singleton()
    {
        assert(!instance_);

        // クラスが多重継承をしている状態で Singleton<T> を使用していた場合に
        // アドレスがずれても本来のクラスアドレスを正しく求めるテクニック
        // 仮のアドレスの base をシングルトンクラスに upcast したときに変化するアドレス変化量を差分計算
        auto base   = reinterpret_cast<T*>(1);
        auto offset = reinterpret_cast<std::uintptr_t>(base) - reinterpret_cast<std::uintptr_t>((Singleton<T>*)base);

        // this の位置から offset の差分移動した位置に本来のクラスが配置されている
        instance_ = reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

    //! デストラクタ
    virtual ~Singleton()
    {
        assert(instance_);
        instance_ = nullptr;
    }

    //!@}
    //----------------------------------------------------------
    //! @name   参照
    //----------------------------------------------------------
    //!@{

    //! 唯一のインスタンスを取得
    static T* instance()
    {
        assert(instance_);
        return instance_;
    }

    //!@}
private:
    //----------------------------------------------------------
    //! @name   copy禁止 & move禁止
    //----------------------------------------------------------
    //!@{

    Singleton(const Singleton<T>&)           = delete;
    Singleton operator=(const Singleton<T>&) = delete;
    Singleton(Singleton<T>&&)                = delete;
    Singleton operator=(Singleton<T>&&)      = delete;

    //!@}
};
