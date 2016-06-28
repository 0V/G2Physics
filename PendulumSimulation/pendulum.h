#pragma once

#ifndef __PENDULUMS_H__
#define __PENDULUMS_H__


#include <vector>
#include <random>

namespace pendulum {

  const double PI = 3.14159265358979323846;

  const double g = 9.8;

  struct PendulumStateDiff;
  struct PendulumState {
    double theta[3];
    double dtheta[3];
    PendulumState();
    void addDiff(const PendulumState p, const PendulumStateDiff pd, double r);
    void adjust();
  };
  struct PendulumStateDiff {
    double diff[6];
    PendulumStateDiff();
    PendulumStateDiff(const PendulumState& p, const double accel[3], const double dt);
  };

  PendulumState::PendulumState() {
    theta[0] = 3.0;
    theta[1] = 3.5;
    theta[2] = 1.5;
    dtheta[0] = 0.8;
    dtheta[1] = 0.5;
    dtheta[2] = -9.0;
  }
  void PendulumState::addDiff(const PendulumState p, const PendulumStateDiff pd, double r) {
    for (int i = 0; i < 3; i++) {
      theta[i] = p.theta[i] + r*pd.diff[i];
      dtheta[i] = p.dtheta[i] + r*pd.diff[i + 3];
    }
  }
  void PendulumState::adjust() {
    for (int i = 0; i < 3; i++) {
      if (theta[i] < 0.0) theta[i] += 2 * PI;
      else if (theta[i] > 2 * PI) theta[i] -= 2 * PI;
    }
  }
  PendulumStateDiff::PendulumStateDiff() {};
  PendulumStateDiff::PendulumStateDiff(const PendulumState& p, const double accel[3], const double dt) {
    for (int i = 0; i < 3; i++) {
      diff[i] = dt*p.dtheta[i];
      diff[i + 3] = dt*accel[i];
    }
  }

  class TriplePendulum {
  private:
    double length[3];
    double mass[3];
    double dt = 0.001;
    double fulcrumX = 0;
    double fulcrumY = 0;

    PendulumState pendl;

    double currentLength[3];
    double currentMass[3];
    PendulumState currentPendl;


    // referenced: http://elephnote.com/blog/archives/236
    void calcRungeKutta() {

      double accel[3];
      PendulumState ps;
      calcAngularAccel(pendl, accel);
      PendulumStateDiff k1(pendl, accel, dt);

      ps.addDiff(pendl, k1, 1.0 / 2.0);

      calcAngularAccel(ps, accel);
      PendulumStateDiff k2(ps, accel, dt);

      ps.addDiff(pendl, k2, 1.0 / 2.0);

      calcAngularAccel(ps, accel);
      PendulumStateDiff k3(ps, accel, dt);

      ps.addDiff(pendl, k3, 1.0);

      calcAngularAccel(ps, accel);
      PendulumStateDiff k4(ps, accel, dt);

      PendulumStateDiff k;
      for (int i = 0; i < 6; i++) {
        k.diff[i] = 1.0 / 6.0*(k1.diff[i] + 2.0*k2.diff[i] + 2.0*k3.diff[i] + k4.diff[i]);
      }
      pendl.addDiff(pendl, k, 1.0);
      return;
    }

    //calculation of generalized acceleration
    void calcAngularAccel(const PendulumState& pt, double accel[3]) {

      int N = 3; // the number of joint
      Eigen::MatrixXd mat(N, N); // coefficient matrix
      Eigen::VectorXd b(N), ans(N); // right matrix, answer


      // sum mass
      double sm[3] =
      {
        mass[0] + mass[1] + mass[2], //m1 + m2 + m3 [0]
        mass[1] + mass[2], //m2 + m3 [1]
        mass[2] // m3 [2]
      };

      // calculated angles
      double ang[3] =
      {
        pt.theta[0] - pt.theta[1],
        pt.theta[1] - pt.theta[2],
        pt.theta[2] - pt.theta[0],
      };

      //permutations A-F, (1,1), (2,2), (3,3)
      double perms[9] =
      {
        sm[1] * length[0] * length[1] * cos(ang[0]),
        sm[2] * length[0] * length[2] * cos(ang[2]),
        sm[2] * length[1] * length[2] * cos(ang[1]),
        sm[1] * length[0] * length[1] * sin(ang[0]),
        sm[2] * length[0] * length[2] * sin(ang[2]),
        sm[2] * length[1] * length[2] * sin(ang[1]),
        sm[0] * length[0] * length[0], // (1,1)
        sm[1] * length[1] * length[1], // (2,2)
        sm[2] * length[2] * length[2], // (3,3)
      };

      // left hand side
      mat <<
        perms[6], perms[0], perms[1],
        perms[0], perms[7], perms[2],
        perms[1], perms[2], perms[8];

      // right hand side
      b <<
        -(sm[0] * g * length[0] * sin(pt.theta[0]))
        - perms[3] * pt.dtheta[1] * pt.dtheta[1]
        + perms[4] * pt.dtheta[2] * pt.dtheta[2],

        -(sm[1] * g * length[1] * sin(pt.theta[1]))
        - perms[5] * pt.dtheta[2] * pt.dtheta[2]
        + perms[3] * pt.dtheta[0] * pt.dtheta[0],

        -(sm[2] * g * length[2] * sin(pt.theta[2]))
        - perms[4] * pt.dtheta[0] * pt.dtheta[0]
        + perms[5] * pt.dtheta[1] * pt.dtheta[1];


      Eigen::FullPivLU<Eigen::MatrixXd> solver(mat);
      //      Eigen::VectorXd b = Eigen::Map<Eigen::VectorXd>(forceVec, 3);

      ans = solver.solve(b);
      accel[0] = ans[0];
      accel[1] = ans[1];
      accel[2] = ans[2];

    }

    std::mt19937 mt;
    std::uniform_real_distribution<> randLength;
    std::uniform_real_distribution<> randMass;
    std::uniform_real_distribution<> randTheta;
    std::uniform_real_distribution<> randDTheta;

  public:

     void initData() {
      length[0] = 0.60;
      length[1] = 0.2;
      length[2] = 0.2;
      mass[0] = 1;
      mass[1] = 2;
      mass[2] = 2;
    }

     void initDataRandomPendl() {
      length[0] = randLength(mt);
      length[1] = randLength(mt);
      length[2] = randLength(mt);
      mass[0] = randMass(mt);
      mass[1] = randMass(mt);
      mass[2] = randMass(mt);
    }

     void initDataRandomTheta() {
      pendl.theta[0] = randTheta(mt);
      pendl.theta[1] = randTheta(mt);
      pendl.theta[2] = randTheta(mt);
      pendl.dtheta[0] = randDTheta(mt);
      pendl.dtheta[1] = randDTheta(mt);
      pendl.dtheta[2] = randDTheta(mt);
    }

     void initRandGen() {
      std::random_device rnd;
      mt = std::mt19937(rnd());
      randLength = std::uniform_real_distribution<>(0.1, 0.5);
      randMass = std::uniform_real_distribution<>(0.5, 5);
      randTheta = std::uniform_real_distribution<>(1, 4);
      randDTheta = std::uniform_real_distribution<>(-10, 10);
    }

    TriplePendulum()
    {
      initRandGen();
      initData();
    };

    TriplePendulum(double diffTime)
    {
      initRandGen();
      dt = diffTime;
      initData();
    };

    void updateCurrent() {
      currentPendl = pendl;
      for (int i = 0; i < 3; i++)
      {
        currentMass[i] = mass[i];
        currentLength[i] = length[i];
      }
    }

    void loadCurrent() {
      currentPendl = pendl;
      for (int i = 0; i < 3; i++)
      {
        currentMass[i] = mass[i];
        currentLength[i] = length[i];
      }
    }

    void reset() {
      pendl = PendulumState();
      initData();
      updateCurrent();
    }

    void resetCurrent() {
      loadCurrent();
    }

    void resetRandomAll() {
      pendl = PendulumState();
      initDataRandomPendl();
      initDataRandomTheta();
      updateCurrent();
    }

    void resetRandomPendl() {
      pendl = PendulumState();
      initDataRandomPendl();
      updateCurrent();
    }

    void resetRandomTheta() {
      pendl = PendulumState();
      initDataRandomTheta();
      updateCurrent();
    }

    /// index : 0-2
    inline double getLength(int index)
    {
      return length[index];
    };
    inline double getMass(int index)
    {
      return mass[index];
    };    /// index : 0-2
    inline double getInitTheta(int index)
    {
      return pendl.theta[index];
    };

    inline double getInitDTheta(int index)
    {
      return pendl.dtheta[index];
    };

    double getDIffTime() {
      return dt;
    }

    double setLength(double len1, double len2, double len3) {
      length[0] = len1;
      length[1] = len2;
      length[2] = len3;
    };


    inline void setLength(double(&_length)[3]) {
      for (int i = 0; i < 3; i++)
      {
       length[i] = _length[i];
      }
    };

    inline void setMass(double m1, double m2, double m3) {
      mass[0] = m1;
      mass[1] = m2;
      mass[2] = m3;
    };

    inline void setMass(double(&_mass)[3]) {
      for (int i = 0; i < 3; i++)
      {
        mass[i] = _mass[i];
      }
    };


    inline void setTheta(double theta0, double theta1, double theta2) {
      pendl.theta[0] = theta0;
      pendl.theta[1] = theta1;
      pendl.theta[2] = theta2;
    };


    inline void setTheta(double(&theta)[3]) {
      for (int i = 0; i < 3; i++)
      {
        pendl.theta[i] = theta[i];
      }
    };

    inline void setDTheta(double dtheta0, double dtheta1, double dtheta2) {
      pendl.dtheta[0] = dtheta0;
      pendl.dtheta[1] = dtheta1;
      pendl.dtheta[2] = dtheta2;
    };

    inline void setDTheta(double(&dtheta)[3]) {
      for (int i = 0; i < 3; i++)
      {
        pendl.dtheta[i] = dtheta[i];
      }
    };

    inline void setDiffTime(double diffTime) {
      dt = diffTime;
    }

    void calcNext()
    {
      calcRungeKutta();
      pendl.adjust();
    };

    //return vertices of pendulums (contains 4 pairs)Pi
    std::vector<std::pair<double, double> > vertices() {
      std::vector<std::pair<double, double> > ans;
      std::pair<double, double> tmpV(0.0, 0.0);
      ans.push_back(tmpV);
      for (int i = 0; i < 3; i++) {
        tmpV.first += length[i] * sin(pendl.theta[i]);
        tmpV.second -= length[i] * cos(pendl.theta[i]);
        ans.push_back(tmpV);
      }
      return ans;
    }
  };
}




#endif