
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "schrodinger.h"

using namespace std;
using namespace schrodinger;

// TODO: いろんなポテンシャル
//     : 次元なおす
//     : 確率密度・規格化
//     : パラメータいじる
int main(int argc, char** argv)
{
  //  string fileName = "phys_data.txt";
  //  if (argc > 1) fileName = argv[1];
  //  read_data(fileName);


  int rangex = 3;

  // Debug
  /*
  std::ofstream ofs0("data0.txt");
  SchrodingerEquation tse(new RightSideFunctionWellPotential(-49.72));
  for (int m = 0; m <  (rangex - (-rangex)) * 1000; m++)
  {
    tse.next();
    ofs0 << tse.getCurrentX() << "  " << tse.getCurrentY() << std::endl;
  }
  return 0; //*/

  int count = (rangex - (-rangex)) * 1000;
  double dE = -1;
  double E = -10;
  double tmpY = 0, oldY = 0;
  for (int i = 0; i < 12; i++)
  {
    double tmpY = 0, oldY = 0;
    dE /= -10;
    int j = 0;
    while (true)
    {
      if(j != 0) E += dE;
      SchrodingerEquation se(new RightSideFunctionWellPotential(E));
      for (int j = 0; j < count; j++)
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
    cout << fixed << setprecision(12) << (double)E << endl;

  }
  std::ofstream ofs("data.txt");
  SchrodingerEquation se(new RightSideFunctionWellPotential(E));
  for (int j = 0; j < count; j++)
  {
    se.next();
//    cout << scientific << showpos << se.getCurrentY() << se.getCurrentY() << endl;
    ofs << fixed << setprecision(12) << se.getCurrentX() << "  " << se.getCurrentY() << "  " << se.getV() << "  " << se.getE() << std::endl;
  }

  std::ofstream ofs2("eigen.txt");

  ofs2 << fixed << setprecision(12) << (double)E;
  return 0;

}


void read_data(string fileName) {
  // TODO: 外部設定ファイル
  /*
  std::ifstream ifs(fileName);/
  std::string str;
  while (std::getline(ifs, str)) {
  std::cout << str << std::endl;
  }
  */

  std::ifstream ifs(fileName);
  std::string str;
  while (getline(ifs, str))
  {
    std::string tmp;
    std::istringstream stream(str);
    while (getline(stream, tmp, ','))
    {
      std::cout << tmp << std::endl;
    }
  }
}