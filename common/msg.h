#ifndef PROJECT_MSG_H
#define PROJECT_MSG_H

namespace BackTraderCN
{
    enum class MsgType: int
    {
        Error = 0,

        Quote = 101,
        Entrust = 102,
        Transaction = 103,

        OrderInput = 201,
        OrderAction = 202,
        Order = 203,
        Trade = 204,
        Position = 205,
        AccountInfo = 206,
        Portfolio = 207,
        AccountInfoByMin = 208,
        PortfolioByMin = 209,
        PositionDetail = 210,
        SubPortfolioInfo = 211,

        RspOrderInput = 251,
        RspOrderAction = 252,

        ReqLogin = 301,
        Subscribe = 302,
        RspLogin = 303,
        RspSubscribe = 304,

        GatewayState = 401,

        AlgoOrderInput = 501,
        AlgoOrderStatus = 502,
        AlgoOrderAction = 503,

        SwitchDay = 601,
        RspTradingDay = 602,

        ReloadFutureInstrument = 701
    };
}

#endif //PROJECT_MSG_H
