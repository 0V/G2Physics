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
      return 2 * (getV(x) - getE(x)) * y;
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

  class RightSideFunctionInclinedWellPotential : public RightSideFunction
  {
  private:
    double _E = 0;
    double _V = -50;
    double range = 2;
    double sl = 1;
  public:
    RightSideFunctionInclinedWellPotential(double E, double slope)
    {
      _E = E;
      sl = slope;
    }

    double getV(double x) override
    {
      double v = 0;
      if (x >= -range && x <= range)
      {
        v = _V + sl*x;
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

    void setSlope(double slope)
    {
      sl = slope;
    }


  };

  class RightSideFunctionDoubleWellPotential : public RightSideFunction
  {
  private:
    double _E = 0;
    double _V[2] = { -50 ,-50 };
    double range[4] = { -2,-1,0,1 };
  public:
    RightSideFunctionDoubleWellPotential(double E)
    {
      _E = E;
    }

    RightSideFunctionDoubleWellPotential(double E, double rangeX[4])
    {
      _E = E;

      for (int i = 0; i < 4; i++) {
        range[i] = rangeX[i];
      }
    }

    double getV(double x) override
    {
      double v = 0;
      if (x >= range[0] && x <= range[1])
      {
        v = _V[0];
      }
      else if (x >= range[2] && x <= range[3])
      {
        v = _V[1];
      }

      return v;
    }

    double getE(double x) override
    {
      return _E;
    }


    void setV(double v) override
    {
      _V[0] = _V[1] = v;
    }
    void setE(double u) override
    {
      _E = u;
    }

    void setV(double v[2])
    {
      _V[0] = v[0];
      _V[1] = v[1];
    }
    void setRange(double rangeX[4])
    {

      for (int i = 0; i < 4; i++) {
        range[i] = rangeX[i];
      }

    }

  };

}

#endif