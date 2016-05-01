#pragma once

#include "sch_function.h"

#ifndef __SCHRODINGER_H__
#define __SCHRODINGER_H__

namespace schrodinger
{

  // default: electron
  struct Dimentionless
  {
    double a = 1;
    double m = 9.10938356; // * 10^-31
    double hbar = 1.054571800;  // * 10^-16
    double ma2divhbar2;
    double hbar2divma2;

    Dimentionless()
    {
      init();
    }

    void init()
    {
      ma2divhbar2 = m * a * a / (hbar * hbar) * 10;
      hbar2divma2 = (hbar * hbar) / (m * a * a * 10);
    }
  };

  class SchrodingerEquation
  {
  private:

    /// dimentionless electron
    Dimentionless Particle;
    RightSideFunction* rsf;
    double dx = 0.001;
    double x = -3;
    double y = 0;
    double p = 1;

    // qキーが壊れていてつらい
    void runge_kutta()
    {
      double dy[4];
      double dp[4];

      dp[0] = dx * rsf->f(x, y);
      dy[0] = dx * p;

      dp[1] = dx * rsf->f(x + dx*0.5, y + dy[0] * 0.5);
      dy[1] = dx * (p + dp[0] / 2);

      dp[2] = dx * rsf->f(x + dx*0.5, y + dy[1] * 0.5);
      dy[2] = dx * (p + dp[1] / 2);

      dp[3] = dx * rsf->f(x + dx, y + dy[2]);
      dy[3] = dx * (p + dp[2]);

      x += dx;
      y += (dy[0] + dy[1] * 2.0 + dy[2] * 2.0 + dy[3]) / 6.0;
      p += (dp[0] + dp[1] * 2.0 + dp[2] * 2.0 + dp[3]) / 6.0;

    }

  public:
    explicit SchrodingerEquation(RightSideFunction* f)
    {
      rsf = f;
//      rsf->setE(rsf->getE(0) * Particle.ma2divhbar2);
//      rsf->setV(-50 * Particle.ma2divhbar2);
    }

    void next()
    {
      runge_kutta();
    }

    void setCurrentX(double cX)
    {
      x = cX;
    }

    void setCurrentY(double cY)
    {
      y = cY;
    }

    double getCurrentX() const
    {
      return x;
    }
    double getCurrentY() const
    {
      return y;
    }
    double getV() const
    {
      return rsf->getV(x) * Particle.hbar2divma2;
    }
    double getE() const
    {
      return rsf->getE(x) * Particle.hbar2divma2;
    }
  };

}

#endif
