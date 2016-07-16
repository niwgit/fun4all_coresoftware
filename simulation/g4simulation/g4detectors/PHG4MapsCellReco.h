#ifndef PHG4MAPSCELLRECO_H
#define PHG4MAPSCELLRECO_H

#include <fun4all/SubsysReco.h>
#include <phool/PHTimeServer.h>
#include <string>
#include <map>
#include <vector>

class PHCompositeNode;
class PHG4CylinderCell_MAPS;

class PHG4MapsCellReco : public SubsysReco
{
 public:

  PHG4MapsCellReco(const std::string &name);

  virtual ~PHG4MapsCellReco(){}
  
  //! module initialization
  int InitRun(PHCompositeNode *topNode);
  
    //! event processing
  int process_event(PHCompositeNode *topNode);
  
  //! end of process
  int End(PHCompositeNode *topNode);
  
  void Detector(const std::string &d) {detector = d;}
  void checkenergy(const int i=1) {chkenergyconservation = i;}

  void set_pixel_x(double pixel_x_in) {pixel_x = pixel_x_in ;} 
  void set_pixel_y(double pixel_y_in) {pixel_y = pixel_y_in ;} 

 protected:
  //void set_size(const int i, const double sizeA, const int sizeB, const int what);
  int CheckEnergy(PHCompositeNode *topNode);
  static std::pair<double, double> get_etaphi(const double x, const double y, const double z);
  static double get_eta(const double radius, const double z);
  std::map<int, int>  binning;
  std::map<int, std::pair <double,int> > cell_size; // cell size in eta/nslats
  std::map<int, std::pair <double,double> > zmin_max; // zmin/zmax for each layer for faster lookup
  std::map<int, double> etastep;
  std::string detector;
  std::string hitnodename;
  std::string cellnodename;
  std::string geonodename;
  std::string seggeonodename;
  PHTimeServer::timer _timer;
  int nbins[2];
  int nslatscombined;
  int chkenergyconservation;
  int layer;

  double pixel_x;
  double pixel_y;

  //std::map<unsigned int, PHG4CylinderCell *> celllist;
  std::map<std::string, PHG4CylinderCell_MAPS*> celllist;  // This map holds the hit cells
};

#endif
