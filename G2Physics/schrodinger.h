#pragma once

#include "sch_function.h"

#ifndef __SCHRODINGER_H__
#define __SCHRODINGER_H__

namespace schrodinger
{
  class SchrodingerEquation
  {
  private:
    RightSideFunction* rhf;
    double dx = 0.001;
    double x = -3;
    double y = 0;
    double p = 1;

    // qキーが壊れていてつらい
    void runge_kutta()
    {
      double dy[4];
      double dp[4];

      dp[0] = dx * rhf->f(x, y);
      dy[0] = dx * p;

      dp[1] = dx * rhf->f(x + dx*0.5, y + dy[0] * 0.5);
      dy[1] = dx * (p + dp[0]/2);

      dp[2] = dx * rhf->f(x + dx*0.5, y + dy[1] * 0.5);
      dy[2] = dx * (p + dp[1] / 2);

      dp[3] = dx * rhf->f(x + dx, y + dy[2]);
      dy[3] = dx * (p + dp[2]);

      x += dx;
      y += (dy[0] + dy[1] * 2.0 + dy[2] * 2.0 + dy[3]) / 6.0;
      p += (dp[0] + dp[1] * 2.0 + dp[2] * 2.0 + dp[3]) / 6.0;

    }

  public:
    explicit SchrodingerEquation(RightSideFunction* f)
    {
      rhf = f;
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
      x = cY;
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
      return rhf->getV(x);
    }
    double getE() const
    {
      return rhf->getE(x);
    }
  };

}

#endif
