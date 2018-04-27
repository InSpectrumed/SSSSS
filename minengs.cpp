#include "minengs.h"
#include "map"

MinengS::MinengS()
{

   m_iGOutMax = m_iGIn = m_iGOut = 0.0;
   m_iColNo = m_iColD = 0;
   m_iPlas = 1;
   m_iKolD = 1;

    m_iDPrev = 0.0;

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
    //qDebug() << m_iGOutMax  << " => " <<  -1 * m_iGOut ;

    qDebug() << m_iGOutMax << " => " <<  -1.0 * m_iGOut
             << " + " <<  m_iColD
             << "$ = " <<  QString::number(-1.0 * m_iGOut + m_iColD * 27.9, 'f', 4)
             << " => " <<  QString::number(( (-1.0 * m_iGOut + m_iColD * 27.9) * 100 / m_iGOutMax)/2.0, 'f', 4);
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

        //qDebug() << a_iOst << "$ => " << QString::number(-1 * m_iGOut + a_iOst * 27.9, 'f', 4) ;

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

   fVar_1(a_dB_In, a_dB_Out);
}

void MinengS::fVar_1(double _bB_in, double _dB_Out)
{
    //qDebug() <<  QString::number(a_dB_In, 'f', 4) << " / " << QString::number(a_dB_Out, 'f', 4);
    //qDebug() <<  a_dB_In << " / " << a_dB_Out;

 // покупка

    //if(_bB_in > (m_iDPrev + 0.0) )
    {
        std::pair<std::map<double, unsigned int>::iterator,bool> ret;
        ret = m_amapCurs.insert ( std::pair<double, unsigned int>(_dB_Out, m_iKolD * m_iPlas) );
        if (ret.second == false)
        {
              ret.first->second += (m_iKolD*m_iPlas);
        }

        m_iGOut += (_dB_Out * m_iKolD * m_iPlas);

        if(m_iGOut > m_iGOutMax)
        {
            m_iGOutMax = m_iGOut;
        }

        m_iColD += (m_iKolD * m_iPlas);
    }

    m_iDPrev = _bB_in;

 // продажа

    unsigned int a_iCol = 0;
    unsigned int a_iNum = 0;

    std::map<double, unsigned int>::iterator it = m_amapCurs.begin();
    for (it=m_amapCurs.begin(); it!=m_amapCurs.end(); ++it)
    {
        if(it->first > _bB_in )
        {
            break;
        }

        a_iNum++;
        a_iCol += it->second;
    }

    if(a_iNum > 0)
    {
        m_iGIn += (a_iCol * _bB_in);

        m_iGOut -= (a_iCol * _bB_in);

        m_iColD -= a_iCol;

        m_amapCurs.erase(m_amapCurs.begin(), it);

        m_iPlas = 1;
    }
    else
    {
        m_iPlas++;
    }
}


void MinengS::fVar_2(double _bB_In, double _dB_Out)
{
    //qDebug() <<  QString::number(a_dB_In, 'f', 4) << " / " << QString::number(a_dB_Out, 'f', 4);
    //qDebug() <<  a_dB_In << " / " << a_dB_Out;


 // покупка

    if(_dB_Out - m_iDPrev >= 0)
    {
        //if(dB_Out - m_iDPrev > 0.5)
        {
            if(g_bPay)
            {
                g_bPay = false;

                std::pair<std::map<double, unsigned int>::iterator,bool> ret;
                ret = m_amapCurs.insert ( std::pair<double, unsigned int>(_dB_Out, m_iKolD) );
                if (ret.second == false)
                {
                      ret.first->second += (m_iKolD);
                }

                m_iGOut += (_dB_Out * m_iKolD);

                if(m_iGOut > m_iGOutMax)
                {
                    m_iGOutMax = m_iGOut;
                }

                m_iColD += m_iKolD;
            }
        }
    }
    else // продажа
    {
        if(!g_bPay)
        {
            // продажа

               unsigned int a_iCol = 0;

               std::map<double, unsigned int>::iterator it = m_amapCurs.begin();
               for (it=m_amapCurs.begin(); it!=m_amapCurs.end(); ++it)
               {
                   if(it->first > _bB_In )
                   {
                       break;
                   }

                   a_iCol += it->second;
               }

               if(a_iCol > 0)
               {
                   m_iGOut -= (a_iCol * _bB_In);

                   m_iColD -= a_iCol;

                   m_amapCurs.erase(m_amapCurs.begin(), it);

                   m_iPlas = 1;
               }
        }
        g_bPay = true;
    }

     m_iDPrev = _dB_Out;




}

void MinengS::fVar_3(double _bB_In, double _dB_Out)
{
    //qDebug() <<  QString::number(a_dB_In, 'f', 4) << " / " << QString::number(a_dB_Out, 'f', 4);
    //qDebug() <<  a_dB_In << " / " << a_dB_Out;


 // покупка

    if(_dB_Out - m_iDPrev >= 0)
    {
        //if(dB_Out - m_iDPrev > 0.5)
        {
            if(g_bPay)
            {
                g_bPay = false;

                std::pair<std::map<double, unsigned int>::iterator,bool> ret;
                ret = m_amapCurs.insert ( std::pair<double, unsigned int>(_dB_Out, m_iKolD * m_iPlas) );
                if (ret.second == false)
                {
                      ret.first->second += (m_iKolD * m_iPlas);
                }

                m_iGOut += (_dB_Out * m_iKolD * m_iPlas);

                if(m_iGOut > m_iGOutMax)
                {
                    m_iGOutMax = m_iGOut;
                }

                m_iColD += (m_iKolD * m_iPlas);
            }
        }
    }
    else // продажа
    {
        g_bPay = true;
    }

     m_iDPrev = _dB_Out;


     unsigned int a_iCol = 0;
     std::map<double, unsigned int>::iterator it = m_amapCurs.begin();
     for (it=m_amapCurs.begin(); it!=m_amapCurs.end(); ++it)
     {
         if(it->first > _bB_In )
         {
             break;
         }

         a_iCol += it->second;
     }

     if(a_iCol > 0)
     {
         m_iGOut -= (a_iCol * _bB_In);

         m_iColD -= a_iCol;

         m_amapCurs.erase(m_amapCurs.begin(), it);

         m_iPlas = 1;
     }
     else
     {
         //m_iPlas++;
     }

}
