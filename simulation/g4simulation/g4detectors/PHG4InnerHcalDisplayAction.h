// Tell emacs that this is a C++ source
// This file is really -*- C++ -*-.
#ifndef G4DETECTORS_PHG4INNERHCALDISPLAYACTION_H
#define G4DETECTORS_PHG4INNERHCALDISPLAYACTION_H

#include <g4main/PHG4DisplayAction.h>

#include <map>
#include <set>
#include <vector>

class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;
class PHParameters;

class PHG4InnerHcalDisplayAction : public PHG4DisplayAction
{
 public:
  PHG4InnerHcalDisplayAction(const std::string &name);

  virtual ~PHG4InnerHcalDisplayAction();

  void ApplyDisplayAction(G4VPhysicalVolume *physvol);
  void SetMyTopVolume(G4VPhysicalVolume *vol) { m_MyTopVolume = vol; }
  void AddScintiVolume(G4LogicalVolume *vol) { m_ScintiLogVolSet.insert(vol);}
  void AddSteelVolume(G4LogicalVolume *vol) {m_SteelVol = vol;}

 private:
  G4VPhysicalVolume *m_MyTopVolume;
  G4LogicalVolume *m_SteelVol;
  std::vector<G4VisAttributes *> m_VisAttVec;
  std::set<G4LogicalVolume *> m_ScintiLogVolSet;
};

#endif  // G4DETECTORS_PHG4INNERHCALDISPLAYACTION_H
