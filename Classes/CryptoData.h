#ifndef _CRYPTO_DATA_H_
#define _CRYPTO_DATA_H_

template<typename TM>
struct CryptoMethod
{
    typedef unsigned int tKey;
    union UCrypto
    {
        TM data;
        tKey cryptodata;
    };
    
    static tKey key()
    {
        static tKey key = []()
        {
            timeval now;
            gettimeofday(&now,  0);
            
            return TM((tKey)now.tv_sec);
        }();
        return key;
    }
    
#ifdef DEBUG
    inline static TM crypto(const TM& data)
    {
        return data;
    }
#else
    inline static TM crypto(const TM& data)
    {
        UCrypto c;
        c.data = data;
        c.cryptodata = c.cryptodata ^ key();
        return c.data;
    }
#endif
};


template<typename T>
class CryptoData
{
    
public:
    typedef T tData;
    
    inline static tData crypto(const tData& data)
    {
        return CryptoMethod<tData>::crypto(data);
    }
    
    
    CryptoData():m_pData(new(T))
    {
    }
    
    ~CryptoData()
    {
        m_pData ? delete(m_pData) : (void)0;
    }
    
    template<typename TData>
    CryptoData(const TData & rData):m_pData(new(T))
    {
        *m_pData = crypto((TData)rData);
    }
    
    CryptoData(const tData& data):m_pData(new(T))
    {
        *m_pData = crypto(data);
    }
    
    CryptoData(const CryptoData<tData>& r):m_pData(new(T))
    {
        *m_pData = *(r.m_pData);
    }
    
    CryptoData(CryptoData<tData>&& r):m_pData(r.m_pData)
    {
        r.m_pData = nullptr;
    }

    inline CryptoData& operator = (const CryptoData<tData>& r)
    {
        *m_pData = *(r.m_pData);
        return *this;
    }
    
    
    template<typename TOther>
    inline CryptoData& operator = (const CryptoData<TOther>& r)
    {
        set(r.get());
        return *this;
    }

    inline CryptoData& operator = (CryptoData<tData>&& r)
    {
        delete(m_pData);
        m_pData = r.m_pData;
        r.m_pData = nullptr;
        return *this;
    }
    
    
    template<typename TOther>
    inline operator TOther()const
    {
        return (TOther)get();
    }
    
    inline tData get()const
    {
        return crypto(*m_pData);
    }

    inline void set(const tData& data)
    {
        delete(m_pData);
        m_pData = new(T);
        *m_pData = crypto(data);
    }
    
    CryptoData& operator++()
    {
        set(get()+1);
        return *this;
    }
    
    CryptoData operator++(int)
    {
        CryptoData data = *this;
        set(get()+1);
        return data;
    }
    
    CryptoData& operator--()
    {
        set(get()-1);
        return *this;
    }
    
    CryptoData operator--(int)
    {
        CryptoData data = *this;
        set(get()-1);
        return data;
    }
    
    
    //compare
    inline bool operator<(const tData& r)const
    {
        return get()< (r);
    }
    
    inline bool operator<=(const tData& r)const
    {
        return get()<=(r);
    }
    
    inline bool operator>(const tData& r)const
    {
        return get()>(r);
    }
    
    inline bool operator>=(const tData& r)const
    {
        return get()>=(r);
    }
    
    inline bool operator==(const tData& r)const
    {
        return get()==(r);
    }
    
    inline bool operator!=(const tData& r)const
    {
        return get()!=(r);
    }
    
    
//    //calc
//
//    inline tData operator+(const tData& r)const
//    {
//        return get()+(r);
//    }
//    
//    inline tData operator-(const tData& r)const
//    {
//        return get()-(r);
//    }
//    
//    inline tData operator*(const tData& r)const
//    {
//        return get()*(r);
//    }
//    
//    inline tData operator/(const tData& r)const
//    {
//        return get()/(r);
//    }
    template<typename TR>
    inline CryptoData& operator+= (const TR& r)
    {
        set(get()+(tData)(r));
        return *this;
    }
    
    template<typename TR>
    inline CryptoData& operator-= (const TR& r)
    {
        set(get()-(tData)(r));
        return *this;
    }
    
    template<typename TR>
    inline CryptoData& operator*= (const TR& r)
    {
        set(get()*(tData)(r));
        return *this;
    }
    
    template<typename TR>
    inline CryptoData& operator/= (const TR& r)
    {
        set(get()/(tData)(r));
        return *this;
    }
    
    
private:
 
    T *m_pData;
    
};





template<typename T>
inline bool operator<(const T& l,const CryptoData<T>& r)
{
    return l<(T)r;
}

template<typename T>
inline bool operator<=(const T& l,const CryptoData<T>& r)
{
    return l<=(T)r;
}

template<typename T>
inline bool operator>(const T& l,const CryptoData<T>& r)
{
    return l>(T)r;
}

template<typename T>
inline bool operator>=(const T& l,const CryptoData<T>& r)
{
    return l>=(T)r;
}

template<typename T>
inline bool operator==(const T& l,const CryptoData<T>& r)
{
    return l==(T)r;
}

template<typename T>
inline bool operator!=(const T& l,const CryptoData<T>& r)
{
    return l!=(T)r;
}

template<typename T>
inline T operator+(const T& l,const CryptoData<T>& r)
{
    return l+(T)(r);
}

template<typename T>
inline T operator-(const T& l,const CryptoData<T>& r)
{
    return l-(T)(r);
}

template<typename T>
inline T operator*(const T& l,const CryptoData<T>& r)
{
    return l*(T)(r);
}

template<typename T>
inline T operator/(const T& l,const CryptoData<T>& r)
{
    return (T)l/(r);
}


template<typename T>
inline T operator+(const CryptoData<T>& l,const T& r)
{
    return (T)l+(r);
}

template<typename T>
inline T operator-(const CryptoData<T>& l,const T& r)
{
    return (T)l-(r);
}

template<typename T>
inline T operator*(const CryptoData<T>& l,const T& r)
{
    return (T)l*(r);
}

template<typename T>
inline T operator/(const CryptoData<T>& l,const T& r)
{
    return (T)l/(r);
}







#endif