#include<iostream>
#include<deque>
#include <string>

#include "Dense"
#include "pendulum.h"
#include "freeglut.h"
#include "SimpleIni.h"
#pragma comment( lib, "freeglut.lib" )

using namespace pendulum;

TriplePendulum pendl;


// guilty(for setting)
bool pointsDraw = true;
bool isRunning = true;
std::string settingFileName = "pendl_setting.ini";
int repeat = 1;
int sleepms = 1;
float locusColor[3][3] = { { 0.5, 0.1, 0.1 },{ 0.1, 0.5, 0.1 },{ 0.1, 0.1, 0.5 } };
float ballColor[4][3] = { { 0.8,0.8,0.8 } ,{ 0.8, 0.2, 0.2 },{ 0.2, 0.8, 0.2 },{ 0.2, 0.2, 0.8 } };
float lineColor[3] = { 1,1,1 };
double lineWidth = 3.0;
double locusWidth = 4.0;
double circleSize = 0.02;
int circlePartition = 10;



void writeInitData() {
  std::cout << "N" << ' ';
  std::cout << "Length" << ' ';
  std::cout << "Mass" << ' ';
  std::cout << "InitTheta" << ' ';
  std::cout << "InitDTheta" << ' ';
  std::cout << std::endl;

  for (int i = 0; i < 3; i++)
  {
    std::cout << i << ' ';
    std::cout << pendl.getLength(i) << ' ';
    std::cout << pendl.getMass(i) << ' ';
    std::cout << pendl.getInitTheta(i) << ' ';
    std::cout << pendl.getInitDTheta(i) << ' ';
    std::cout << std::endl;
  }
}

const int PointNum = 300;
std::deque<std::vector<std::pair<double, double> > > points;

void getPoints() {
  points.push_front(pendl.vertices());
  if (points.size() > PointNum) {
    points.pop_back();
  }
}

void changeBallColor(int itr, double(&color)[3]) {
  if (itr >= 4) return;
  for (int i = 0; i < 3; i++)
  {
    ballColor[itr][i] = color[i];
  }
}

void changeLocusColor(int itr, double(&color)[3]) {
  if (itr >= 3) return;
  for (int i = 0; i < 3; i++)
  {
    locusColor[itr][i] = color[i];
  }
}


void loadSetting(std::string fileName) {
  CSimpleIniA ini;
  ini.SetUnicode();
  SI_Error rc = ini.LoadFile(fileName.c_str());
  if (rc < 0) return;

  CSimpleIniA::TNamesDepend sections;

  ini.GetAllSections(sections);
  CSimpleIniA::TNamesDepend::const_iterator i;
  for (i = sections.begin(); i != sections.end(); ++i) {


    //‚±‚±ƒ_ƒT‚¢‚µ‚È‚ñ‚Æ‚©‚µ‚½‚¢Š´‚ ‚é
    if (strcmp(i->pItem, "theta") == 0 ||
      strcmp(i->pItem, "dtheta") == 0 ||
      strcmp(i->pItem, "mass") == 0 ||
      strcmp(i->pItem, "length") == 0 ||
      strcmp(i->pItem, "ballColor0") == 0 ||
      strcmp(i->pItem, "ballColor1") == 0 ||
      strcmp(i->pItem, "ballColor2") == 0 ||
      strcmp(i->pItem, "ballColor3") == 0 ||
      strcmp(i->pItem, "locusColor0") == 0 ||
      strcmp(i->pItem, "locusColor1") == 0 ||
      strcmp(i->pItem, "locusColor2") == 0) {

      double item[3];
      item[0] = ini.GetDoubleValue(i->pItem, "item0");
      item[1] = ini.GetDoubleValue(i->pItem, "item1");
      item[2] = ini.GetDoubleValue(i->pItem, "item2");

      if (strcmp(i->pItem, "theta") == 0)           pendl.setTheta(item);
      else if (strcmp(i->pItem, "dtheta") == 0)     pendl.setDTheta(item);
      else if (strcmp(i->pItem, "mass") == 0)       pendl.setMass(item);
      else if (strcmp(i->pItem, "length") == 0)     pendl.setLength(item);
      else if (strcmp(i->pItem, "ballColor0") == 0) changeBallColor(0, item);
      else if (strcmp(i->pItem, "ballColor1") == 0) changeBallColor(1, item);
      else if (strcmp(i->pItem, "ballColor2") == 0) changeBallColor(2, item);
      else if (strcmp(i->pItem, "ballColor3") == 0) changeBallColor(3, item);
      else if (strcmp(i->pItem, "locusColor0") == 0) changeLocusColor(0, item);
      else if (strcmp(i->pItem, "locusColor1") == 0) changeLocusColor(1, item);
      else if (strcmp(i->pItem, "locusColor2") == 0) changeLocusColor(2, item);
    }
/*  else if (strcmp(i->pItem, "gravity") == 0) {
      double tmp;
      tmp = ini.GetLongValue(i->pItem, "accel");
      if (tmp > 0) pendl.setGravity(tmp);
    }*/
    else if (strcmp(i->pItem, "speed") == 0) {
      int tmp;
      tmp = ini.GetLongValue(i->pItem, "sleepms");
      if (tmp > 0) sleepms = tmp;

      tmp = ini.GetLongValue(i->pItem, "repeat");
      if (tmp > 0) repeat = tmp;
    }
    else if (strcmp(i->pItem, "size") == 0) {
      double tmp;

      tmp = ini.GetDoubleValue(i->pItem, "lineWidth");
      if (tmp > 0) lineWidth = tmp;

      tmp = ini.GetDoubleValue(i->pItem, "locusWidth");
      if (tmp > 0) locusWidth = tmp;

      tmp = ini.GetDoubleValue(i->pItem, "circleSize");
      if (tmp > 0) circleSize = tmp;

      tmp = ini.GetLongValue(i->pItem, "circlePartition");
      if (tmp > 0) circlePartition = tmp;
    }
    else if (strcmp(i->pItem, "lineColor") == 0) {
      lineColor[0] = ini.GetDoubleValue(i->pItem, "item0");
      lineColor[1] = ini.GetDoubleValue(i->pItem, "item1");
      lineColor[2] = ini.GetDoubleValue(i->pItem, "item2");
    }
    else if (strcmp(i->pItem, "bgColor") == 0) {
      double item[4];
      item[0] = ini.GetDoubleValue(i->pItem, "item0");
      item[1] = ini.GetDoubleValue(i->pItem, "item1");
      item[2] = ini.GetDoubleValue(i->pItem, "item2");
      item[3] = ini.GetDoubleValue(i->pItem, "item3");
      glClearColor(item[0], item[1], item[2], item[3]);
    }
  }
}

void init() {
  writeInitData();
  getPoints();
}

void keydown(unsigned char key, int x, int y) {
  switch (key) {
  case 'w':
    pendl.reset();
    init();
    break;
  case 'e':
    pendl.resetCurrent();
    init();
    break;
  case 'r':
    pendl.resetRandomAll();
    init();
    break;
  case 't':
    pendl.resetRandomTheta();
    init();
    break;
  case 'y':
    pendl.resetRandomPendl();
    init();
    break;
  case 'p':
    pointsDraw = !pointsDraw;
    break;
  case 's':
    isRunning = !isRunning;
    break;
  case 'f':
    pendl.reset();
    loadSetting(settingFileName);
    init();
    break;
  case 'q':
  case 'z':
    exit(0);
    break;
  }
}


void timertick(int value)
{
  if (isRunning) {
    for (int i = 0; i < repeat; i++) {
      pendl.calcNext();
      getPoints();
    }
  }
  glutPostRedisplay();
  glutTimerFunc(sleepms, timertick, 0);
}


/*
void drawString(double posX, double posY, unsigned char colorR, unsigned char colorG, unsigned char colorB, std::string str) {
  glColor3ub(colorR, colorG, colorB);
  glRasterPos2d(posX, posY);
  glutBitmapString(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<const unsigned char*>(str.c_str()));
}

void drawString(double posX, double posY, unsigned char colorR, unsigned char colorG, unsigned char colorB, double str) {
  glColor3ub(colorR, colorG, colorB);
  glRasterPos2d(posX, posY);
  glutBitmapString(GLUT_BITMAP_HELVETICA_10, reinterpret_cast<const unsigned char*>(std::to_std::string(str).c_str()));
}
*/

void drawCircle() {
  // ‰~
  for (int k = 0; k < 4; k++) {
    glColor3f(ballColor[k][0], ballColor[k][1], ballColor[k][2]);
    float x, y, z, r;
    int n = circlePartition;//•ªŠ„”
    r = circleSize;//”¼Œa
    glBegin(GL_POLYGON);

    for (int i = 0; i < n; i++) {
      x = r*cos(2.0*PI*(float)i / (float)n) + points.front().at(k).first;
      y = r*sin(2.0*PI*(float)i / (float)n) + points.front().at(k).second;
      z = 0.0;
      glVertex3f(x, y, z);
    }
    glEnd();
  }
}

void drawLocus() {
  if (pointsDraw) {
    glPointSize(1.0);
    glLineWidth(locusWidth);
    for (int i = 0; i < 3; i++) {
      glBegin(GL_LINE_STRIP);
      int pCount = 0;
      for (auto it = points.rbegin(); it != points.rend(); ++it) {
        float colorMul = static_cast<float>(static_cast<double>(pCount) / static_cast<double>(points.size()));
        glColor3f(locusColor[i][0] * colorMul, locusColor[i][1] * colorMul, locusColor[i][2] * colorMul);
        glVertex2d(it->at(i + 1).first, it->at(i + 1).second);
        pCount++;
      }
      glEnd();
    }
  }
}

void drawPendl() {
  glLineWidth(lineWidth);
  glPointSize(1);
  glColor3f(lineColor[0], lineColor[1], lineColor[2]);
  for (int i = 0; i < 3; i++) {
    glBegin(GL_LINES);
    glVertex2d(points.front().at(i).first, points.front().at(i).second);
    glVertex2d(points.front().at(i + 1).first, points.front().at(i + 1).second);
    glEnd();
  }
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawLocus();
  drawPendl();
  drawCircle();

  glutSwapBuffers();
}

int main(int argc, char *argv[])
{
  const int WindowSize = 640;

  if (argc > 1) settingFileName = argv[1];

  glutInit(&argc, argv);
  glutInitWindowSize(WindowSize, WindowSize);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutTimerFunc(10, timertick, 0);
  glutKeyboardFunc(keydown);
  glClearColor(0.0, 0.0, 0.0, 1.0);

  loadSetting(settingFileName);
  init();
  
  glutMainLoop();

  return 0;
}

