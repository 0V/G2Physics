
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "schrodinger.h"

using namespace std;
using namespace schrodinger;

// TODO: ‚¢‚ë‚ñ‚Èƒ|ƒeƒ“ƒVƒƒƒ‹
int main(int argc, char** argv)
{
  // Debug
  /*
  std::ofstream ofs0("data0.txt");
  SchrodingerEquation tse(new RightSideFunctionWellPotential(-49.72));
  for (int m = 0; m <  (rangex - (-rangex)) * 1000; m++)
  {
  tse.next();
  ofs0 << tse.getCurrentX() << "  " << tse.getCurrentY() << std::endl;
  }
  Dimentionless el;
  //  cout << fixed << setprecision(12) << el.hbar2divma2 << endl;
//  cout << fixed << setprecision(12) << el.ma2divhbar2 << endl;
//  cin >> rangex;
  return 0; //*/

  string fileName = "phys_data.txt";
  if (argc > 1) fileName = argv[1];

  ifstream ifs(fileName);
  string str;
  double tmpPhysData[50];
  int tpd = 0;
  while (getline(ifs, str)) {
    cout << str << endl;
    tmpPhysData[tpd] = stod(str);
    tpd++;
  }


  // [-startX,startX+rangeX]
  double E = -50;
  double dE = -1;
  double startX = -5;
  double rangeX = 10;
  double dx = 0.001;
  int accuracy = 12;
  double startP = 1;
  double startY = 0;
  double _V = 1;

  if (tpd > 0) E = tmpPhysData[0];
  if (tpd > 1) dE = tmpPhysData[1];
  if (tpd > 2) startX = tmpPhysData[2];
  if (tpd > 3) rangeX = tmpPhysData[3];
  if (tpd > 4) dx = tmpPhysData[4];
  if (tpd > 5) accuracy = tmpPhysData[5];
  if (tpd > 6) startP = tmpPhysData[6];
  if (tpd > 7) startY = tmpPhysData[7];
  if (tpd > 8) _V = tmpPhysData[8];
  int count = rangeX / dx;



  for (int i = 0; i < accuracy; i++)
  {
    double tmpY = 0;
    double oldY = 0;

    dE /= -10;
    int j = 0;
    while (true)
    {
      if (j != 0) E += dE;
      RightSideFunction* potential = new RightSideFunctionHarmonicPotential(E);
      potential->setV(_V);
      SchrodingerEquation se(potential);
      se.setCurrentX(startX);
      se.setCurrentY(startY);
      se.setCurrentP(startP);
      se.setDX(dx);

      for (int k = 0; k < count; k++)
      {
        se.next();
      }

      oldY = tmpY;
      tmpY = se.getCurrentY();
      if ((j > 0) && ((oldY > 0) && (tmpY < 0))) break;
      if ((j > 0) && ((oldY < 0) && (tmpY > 0))) break;
      if (abs(se.getCurrentY()) < 0.000001) { i = 13; break; }
      j++;
    }

    //check
    cout << fixed << setprecision(12) << E << endl;

  }
  ofstream ofs("data.txt");
  RightSideFunction* potential = new RightSideFunctionHarmonicPotential(E);
  potential->setV(_V);
  SchrodingerEquation se(potential);
  se.setCurrentX(startX);
  se.setCurrentY(startY);
  se.setCurrentP(startP);
  se.setDX(dx);

  double integ = 0;
  for (int j = 0; j < count; j++)
  {
    se.next();
    //    cout << scientific << showpos << se.getCurrentY() << se.getCurrentY() << endl;
    ofs << fixed << setprecision(12) << se.getCurrentX() << "  " << se.getCurrentY() << "  " << se.getV() << "  " << se.getE() << std::endl;
    integ += se.getCurrentY() * se.getCurrentY();
  }

  ofstream ofs2("eigen.txt");

  ofs2 << fixed << setprecision(12) << E << endl;
  ofs2 << fixed << setprecision(12) << integ * 0.001 << endl;
  ofs2 << endl;
  ofs2 << fixed << setprecision(12) << dE << endl;
  ofs2 << fixed << setprecision(12) << startX << endl;
  ofs2 << fixed << setprecision(12) << rangeX << endl;
  ofs2 << fixed << setprecision(12) << dx<< endl;
  ofs2 << fixed << setprecision(12) << accuracy << endl;
  ofs2 << fixed << setprecision(12) << startP << endl;
  ofs2 << fixed << setprecision(12) << startY << endl;
  ofs2 << fixed << setprecision(12) << _V << endl;

  return 0;

}