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
    double m_iGIn;
    double m_iGOut;
    double m_iGOutMax;
    unsigned int m_iColD;
    unsigned int m_iColNo;
    unsigned int m_iPlas;
    unsigned int m_iKolD;
    QFile *m_fUSD;
    std::map<double, unsigned int> m_amapCurs;

    void fPars(QString url);
};

#endif // MINENGS_H
