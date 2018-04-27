#ifndef MINENGS_H
#define MINENGS_H

#include <QString>
#include <QDebug>
#include <QFile>
#include "map"

class MinengS
{
public:
    MinengS();
    ~MinengS();

private:
    bool g_bPay = true;
    double m_iGIn;
    double m_iGOut;
    double m_iGOutMax;
    double m_iDPrev;
    unsigned int m_iColD;
    unsigned int m_iColNo;
    unsigned int m_iPlas;
    unsigned int m_iKolD;
    QFile *m_fUSD;
    std::map<double, unsigned int> m_amapCurs;

    void fPars(QString url);
    void fVar_1(double _bB_in, double _dB_Out);
    void fVar_2(double _bB_in, double _dB_Out);
    void fVar_3(double _bB_in, double _dB_Out);
};

#endif // MINENGS_H
