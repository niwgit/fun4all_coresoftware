#ifndef G4HISTOS_G4EVTTREE_H
#define G4HISTOS_G4EVTTREE_H

#define MAXLAYER 50
#define MAXHIT 500000

#include <Rtypes.h>

typedef struct
{
  // Event Level
  float energy;
  float theta;
  float phi;
  float px;
  float py;
  float pz;

  int cemcactLayers;
  int cemcabsLayers;
  int hcalactLayers;
  int hcalabsLayers;

  float cemcactESum[MAXLAYER];
  float cemcabsESum[MAXLAYER];
  float hcalactESum[MAXLAYER];
  float hcalabsESum[MAXLAYER];

  // Hit level
  int nhits;
  int detid[MAXHIT];
  int layer[MAXHIT];
  int hitid[MAXHIT];
  int scintid[MAXHIT];
  int trkid[MAXHIT];
  float x0[MAXHIT];
  float y0[MAXHIT];
  float z0[MAXHIT];
  float x1[MAXHIT];
  float y1[MAXHIT];
  float z1[MAXHIT];
  float edep[MAXHIT];
  Int_t mcp_id[MAXHIT];
  Int_t pixel_id[MAXHIT];
  Double_t CherenkovMC[MAXHIT];
  Double_t lead_time[MAXHIT];
  Double_t wavelength[MAXHIT];
  Double_t hit_globalPos[MAXHIT][3];
  Double_t hit_localPos[MAXHIT][3];
  Double_t hit_digiPos[MAXHIT][3];
  Double_t hit_mom[MAXHIT][3];
  Double_t hit_pos[MAXHIT][3];
  Long64_t hit_pathId[MAXHIT];

} G4EvtTree;

#endif
