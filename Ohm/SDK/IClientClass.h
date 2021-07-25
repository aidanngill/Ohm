#pragma once

class RecvTable;
class RecvProp;

class RecvTable {
public:
    RecvProp*       m_pProps;
    int	            m_nProps;
    void*           m_pDecoder;
    char*           m_pNetTableName;
    bool            m_bInitialized;
    bool            m_bInMainList;
};

class RecvProp {
public:
    char*           m_pVarName;
    void*           m_RecvType;
    int             m_Flags;
    int             m_StringBufferSize;
    int             m_bInsideArray;
    const void*     m_pExtraData;
    RecvProp*       m_pArrayProp;
    void*           m_ArrayLengthProxy;
    void*           m_ProxyFn;
    void*           m_DataTableProxyFn;
    RecvTable*      m_pDataTable;
    int             m_Offset;
    int             m_ElementStride;
    int             m_nElements;
    const char*     m_pParentArrayPropName;
};

class IClientClass {
public:
    void*           m_pCreateFn;
    void*           m_pCreateEventFn;
    const char*     m_pNetworkName;
    RecvTable*      m_pRecvTable;
    IClientClass*   m_pNext;
    int 			m_ClassID;
};