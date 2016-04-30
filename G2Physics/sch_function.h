#pragma once

#ifndef __SCH_FUNTION_H__
#define __SCH_FUNTION_H__

namespace  schrodinger {
  class RightSideFunction
  {
  public:
    virtual double getV(double x) { return 0; }
    virtual double getE(double x) { return 0; }

    double f(double x, double y)
    {
      return 2*(getV(x) - getE(x)) * y;
    }

    virtual ~RightSideFunction()
    {

    }
  };

  class RightSideFunctionWellPotential : public RightSideFunction
  {
  private:
    double _E = 0;
    double _V = -50;
  public:
    RightSideFunctionWellPotential(double E)
    {
      _E = E;
    }

    double getV(double x) override
    {
      double v = 0;
      if (x >= -2 && x <= 2)
      {
        v = _V;
      }

      return v;
    }

    double getE(double x) override
    {
      return _E;
    }


    void setV(double v)
    {
      _V = v;
    }

    void setE(double u)
    {
      _E = u;
    }


  };
}

#endif