#pragma once

#ifndef __SCH_FUNTION_H__
#define __SCH_FUNTION_H__

namespace  schrodinger {
  class RightSideFunction
  {
  public:
    virtual double getV(double x) { return 0; }
    virtual double getE(double x) { return 0; }
    virtual void setV(double v) { }
    virtual void setE(double u) { }

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
    double range = 2;
  public:
    RightSideFunctionWellPotential(double E)
    {
      _E = E;      
    }

    double getV(double x) override
    {
      double v = 0;
      if (x >= -range && x <= range)
      {
        v = _V;
      }

      return v;
    }

    double getE(double x) override
    {
      return _E;
    }


    void setV(double v) override
    {
      _V = v;
    }

    void setE(double u) override
    {
      _E = u;
    }


  };


  class RightSideFunctionHarmonicPotential : public RightSideFunction
  {
  private:
    double _E = 0;
    double k = 1;
    double range = 2;
  public:
    RightSideFunctionHarmonicPotential(double E)
    {
      _E = E;
    }

    double getV(double x) override
    {
      return (x * x * 0.5 * k);
    }

    double getE(double x) override
    {
      return _E;
    }

    void setV(double kvalue) override
    {
      k = kvalue;
    }

    void setE(double u) override
    {
      _E = u;
    }


  };
}

#endif