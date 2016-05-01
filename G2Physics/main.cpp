
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

  double rangex = 3;
  int count = (rangex - (-rangex)) * 1000;
  double dE = -1;
  double E = -50;

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

  if (tpd > 0) E = tmpPhysData[0];
  if (tpd > 1) dE = tmpPhysData[1];

  for (int i = 0; i < 12; i++)
  {
    double tmpY = 0;
    double oldY = 0;

    dE /= -10;
    int j = 0;
    while (true)
    {
      if (j != 0) E += dE;
      SchrodingerEquation se(new RightSideFunctionWellPotential(E));
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
  SchrodingerEquation se(new RightSideFunctionWellPotential(E));
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
  return 0;

}