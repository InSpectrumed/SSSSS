#include "minengs.h"
#include "map"

MinengS::MinengS()
{

   m_iGOutMax = m_iGIn = m_iGOut = 0.0;
   m_iColNo = m_iColD = 0;
   m_iPlas = 1;
   m_iKolD = 1;

    QFile file("privat.txt");
    if(!file.open(QIODevice::ReadOnly)) {
       qDebug() << "privat.txt -> " << file.errorString();
    }

    int a_iKol = 0;
    while(!file.atEnd())
    {
        QString _sString = file.readLine();
/*
        if( _sString.indexOf("2017-08-18") >= 0)
        {
            break;
        }
*/
        fPars(_sString);


        a_iKol++;
/*
        if(a_iKol > 730)
        {
            break;
        }
        */

    }
    qDebug() << m_iGOutMax  << " => " <<  -1 * m_iGOut ;
}

MinengS::~MinengS()
{
    if(!m_amapCurs.empty())
    {
        int a_iOst = 0;

        std::map<double, unsigned int>::iterator it = m_amapCurs.begin();
        for (it=m_amapCurs.begin(); it!=m_amapCurs.end(); ++it)
        {
            //qDebug() << it->first << " => " << it->second;

            a_iOst += it->second;
        }

        qDebug() << a_iOst << " => " << QString::number(-1 * m_iGOut + a_iOst * 27.9, 'f', 4) ;

        m_amapCurs.clear();
    }
}

void MinengS::fPars(QString _sString)
{
    if(_sString.indexOf("No DATA") >= 0)
    {
        m_iColNo++;
        return;
    }

   int a_iStart = _sString.indexOf(" ") + 1;
   int a_iFin = _sString.indexOf(" ", a_iStart);
   QString a_sC = _sString.mid(a_iStart, a_iFin - a_iStart);
   QString a_sD = a_sC.mid(a_sC.indexOf(",")+1);
   a_sC = a_sC.mid(0, a_sC.indexOf(','));
   double a_dB_In = 0.0 + a_sC.toInt() + a_sD.toDouble()/10000;


   a_iStart = ++a_iFin;
   a_iFin= _sString.size();

   a_sC = _sString.mid(a_iStart, a_iFin - a_iStart);
   a_sD = a_sC.mid(a_sC.indexOf(",")+1);
   a_sC = a_sC.mid(0, a_sC.indexOf(','));
   double a_dB_Out = 0.0 + a_sC.toInt() + a_sD.toDouble()/10000;

   //qDebug() <<  QString::number(a_dB_In, 'f', 4) << " / " << QString::number(a_dB_Out, 'f', 4);
   //qDebug() <<  a_dB_In << " / " << a_dB_Out;

   std::pair<std::map<double, unsigned int>::iterator,bool> ret;
   ret = m_amapCurs.insert ( std::pair<double, unsigned int>(a_dB_Out, m_iKolD * m_iPlas) );
   if (ret.second == false)
   {
         ret.first->second += (m_iKolD*m_iPlas);
   }

   m_iGOut += (a_dB_Out * m_iKolD * m_iPlas);

   if(m_iGOut > m_iGOutMax)
   {
       m_iGOutMax = m_iGOut;
   }


   unsigned int a_iCol = 0;
   unsigned int a_iNum = 0;

   std::map<double, unsigned int>::iterator it = m_amapCurs.begin();
   for (it=m_amapCurs.begin(); it!=m_amapCurs.end(); ++it)
   {
       if(it->first > a_dB_In )
       {
           break;
       }
       a_iNum++;
       a_iCol += it->second;
   }

   if(a_iNum > 0)
   {
       m_iGIn += (a_iCol*a_dB_In);

       m_iGOut -= (a_iCol*a_dB_In);

       m_iColD += a_iCol;

       m_amapCurs.erase(m_amapCurs.begin(), it);

       m_iPlas = 1;
   }
   else
   {
       m_iPlas++;
   }

}
