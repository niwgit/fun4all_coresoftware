#include "G4SnglTree.h"

#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>        // for PHG4HitContainer, PHG4Hit...
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4eicdirc/PrtHit.h>

#include <fun4all/SubsysReco.h>             // for SubsysReco

#include <phool/getClass.h>

#include <TFile.h>
#include <TTree.h>

#include <cmath>                           // for atan2, sqrt
#include <cstring>                         // for strcmp
#include <iostream>                         // for ostringstream, operator<<
#include <sstream>
#include <utility>                          // for pair

using namespace std;

G4SnglTree::G4SnglTree(const std::string &name, const std::string &filename)
  : SubsysReco(name)
  , nblocks(0)
  , _filename(filename)
  , g4tree(nullptr)
  , outfile(nullptr)
{
}

int G4SnglTree::Init(PHCompositeNode *)
{
  outfile = new TFile(_filename.c_str(), "RECREATE");
  g4tree = new TTree("mG4EvtTree", "g4tree");
  g4tree->SetAutoSave(1000000);

  cout << "Initialize Geant 4 Tree ... << " << endl;

  /// Event level
  g4tree->Branch("energy", &mG4EvtTree.energy, "energy/F");
  g4tree->Branch("theta", &mG4EvtTree.theta, "theta/F");
  g4tree->Branch("phi", &mG4EvtTree.phi, "phi/F");
  g4tree->Branch("px", &mG4EvtTree.px, "px/F");
  g4tree->Branch("py", &mG4EvtTree.py, "py/F");
  g4tree->Branch("pz", &mG4EvtTree.pz, "pz/F");
  g4tree->Branch("cemcactLayers", &mG4EvtTree.cemcactLayers, "cemcactLayers/I");
  g4tree->Branch("cemcabsLayers", &mG4EvtTree.cemcabsLayers, "cemcabsLayers/I");
  g4tree->Branch("hcalactLayers", &mG4EvtTree.hcalactLayers, "hcalactLayers/I");
  g4tree->Branch("hcalabsLayers", &mG4EvtTree.hcalabsLayers, "hcalabsLayers/I");
  g4tree->Branch("cemcactESum", mG4EvtTree.cemcactESum, "cemcactESum[cemcactLayers]/F");
  g4tree->Branch("cemcabsESum", mG4EvtTree.cemcabsESum, "cemcabsESum[cemcabsLayers]/F");
  g4tree->Branch("hcalactESum", mG4EvtTree.hcalactESum, "hcalactESum[hcalactLayers]/F");
  g4tree->Branch("hcalabsESum", mG4EvtTree.hcalabsESum, "hcalabsESum[hcalabsLayers]/F");

  /// Hit level
  g4tree->Branch("nhits", &mG4EvtTree.nhits, "nhits/I");
  g4tree->Branch("detid", mG4EvtTree.detid, "detid[nhits]/I");
  g4tree->Branch("layer", mG4EvtTree.layer, "layer[nhits]/I");
  g4tree->Branch("hitid", mG4EvtTree.hitid, "hitid[nhits]/I");
  g4tree->Branch("trkid", mG4EvtTree.trkid, "trkid[nhits]/I");
  g4tree->Branch("scintid", mG4EvtTree.scintid, "scintid[nhits]/I");
  g4tree->Branch("x0", mG4EvtTree.x0, "x0[nhits]/F");
  g4tree->Branch("y0", mG4EvtTree.y0, "y0[nhits]/F");
  g4tree->Branch("z0", mG4EvtTree.z0, "z0[nhits]/F");
  g4tree->Branch("x1", mG4EvtTree.x1, "x1[nhits]/F");
  g4tree->Branch("y1", mG4EvtTree.y1, "y1[nhits]/F");
  g4tree->Branch("z1", mG4EvtTree.z1, "z1[nhits]/F");
  g4tree->Branch("edep", mG4EvtTree.edep, "edep[nhits]/F");
  g4tree->Branch("edep", mG4EvtTree.edep, "edep[nhits]/I");

  g4tree->Branch("mcp_id", mG4EvtTree.mcp_id, "mcp_id[nhits]/I");
  g4tree->Branch("pixel_id", mG4EvtTree.pixel_id, "pixel_id[nhits]/I");
  g4tree->Branch("CherenkovMC", mG4EvtTree.CherenkovMC, "CherenkovMC[nhits]/D");
  g4tree->Branch("lead_time", mG4EvtTree.lead_time,"lead_time[nhits]/D");
  g4tree->Branch("wavelength", mG4EvtTree.wavelength,"wavelength[nhits]/D");
  g4tree->Branch("hit_pathId", mG4EvtTree.hit_pathId, "hit_pathId[nhits]/L");

  g4tree->Branch("hit_globalPos", mG4EvtTree.hit_globalPos, "hit_globalPos[nhits][3]/D");
  g4tree->Branch("hit_localPos", mG4EvtTree.hit_localPos, "hit_localPos[nhits][3]/D");
  g4tree->Branch("hit_digiPos", mG4EvtTree.hit_digiPos, "hit_digiPos[nhits][3]/D");
  g4tree->Branch("hit_mom", mG4EvtTree.hit_mom, "hit_mom[nhits][3]/D");
  g4tree->Branch("hit_pos", mG4EvtTree.hit_pos, "hit_pos[nhits][3]/D");

  return 0;
}

int G4SnglTree::process_event(PHCompositeNode *topNode)
{
  // get the primary particle which did this to us....
  PHG4TruthInfoContainer *truthInfoList = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");

  const PHG4TruthInfoContainer::Range primRange = truthInfoList->GetPrimaryParticleRange();
  double px = primRange.first->second->get_px();
  double py = primRange.first->second->get_py();
  double pz = primRange.first->second->get_pz();
  double e = primRange.first->second->get_e();
  double pt = sqrt(px * px + py * py);
  double phi = atan2(py, px);
  double theta = atan2(pt, pz);

  mG4EvtTree.energy = e;
  mG4EvtTree.theta = theta;
  mG4EvtTree.phi = phi;
  mG4EvtTree.px = px;
  mG4EvtTree.py = py;
  mG4EvtTree.pz = pz;

  int nhits = 0;

  ostringstream nodename;
  set<string>::const_iterator iter;

  for (iter = _node_postfix.begin(); iter != _node_postfix.end(); ++iter)
  {
    int detid = (_detid.find(*iter))->second;
    nodename.str("");
    nodename << "G4HIT_" << *iter;
    PHG4HitContainer *hits = findNode::getClass<PHG4HitContainer>(topNode, nodename.str());

    if (!strcmp("G4HIT_CEMC", nodename.str().c_str()))  //CEMC scintillator
    {
      mG4EvtTree.cemcactLayers = process_hit(hits, "G4HIT_CEMC", detid, nhits);
    }
    else if (!strcmp("G4HIT_ABSORBER_CEMC", nodename.str().c_str()))  //CEMC Aabsorber G4_W
    {
      mG4EvtTree.cemcabsLayers = process_hit(hits, "G4HIT_ABSORBER_CEMC", detid, nhits);
    }
    else if (!strcmp("G4HIT_HCAL", nodename.str().c_str()))  //HCAL Active scintilltor
    {
      mG4EvtTree.hcalactLayers = process_hit(hits, "G4HIT_HCAL", detid, nhits);
    }
    else if (!strcmp("G4HIT_ABSORBER_HCAL", nodename.str().c_str()))  //HCAL Aabsorber steel
    {
      mG4EvtTree.hcalabsLayers = process_hit(hits, "G4HIT_ABSORBER_HCAL", detid, nhits);
    }
    else if (!strcmp("G4HIT_DIRC", nodename.str().c_str())) // DIRC
      {
	process_hit(hits, "G4HIT_DIRC", detid, nhits);
      }
	
  }

  mG4EvtTree.nhits = nhits;

  if (g4tree) g4tree->Fill();

  return 0;
}

int G4SnglTree::End(PHCompositeNode */*topNode*/)
{
  outfile->cd();
  g4tree->Write();
  outfile->Write();
  outfile->Close();
  delete outfile;
  return 0;
}

void G4SnglTree::AddNode(const std::string &name, const int detid)
{
  _node_postfix.insert(name);
  _detid[name] = detid;
  return;
}

int G4SnglTree::process_hit(PHG4HitContainer *hits, const string &dName, int detid, int &nhits)
{
  map<int, double> layer_edep_map;
  map<int, double>::const_iterator edepiter;

  int nLayers = 0;
  if (hits)
  {
    // double esum = 0;
    PHG4HitContainer::ConstRange hit_range = hits->getHits();
    for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)
    {
      PrtHit *dirc_hit = dynamic_cast<PrtHit*>(hit_iter->second);

      layer_edep_map[dirc_hit->get_layer()] += dirc_hit->get_edep();
      mG4EvtTree.detid[nhits] = detid;
      mG4EvtTree.layer[nhits] = dirc_hit->get_layer();
      mG4EvtTree.hitid[nhits] = dirc_hit->get_hit_id();
      mG4EvtTree.trkid[nhits] = dirc_hit->get_trkid();
      mG4EvtTree.scintid[nhits] = dirc_hit->get_scint_id();
      mG4EvtTree.x0[nhits] = dirc_hit->get_x(0);
      mG4EvtTree.y0[nhits] = dirc_hit->get_y(0);
      mG4EvtTree.z0[nhits] = dirc_hit->get_z(0);
      mG4EvtTree.x1[nhits] = dirc_hit->get_x(1);
      mG4EvtTree.y1[nhits] = dirc_hit->get_y(1);
      mG4EvtTree.z1[nhits] = dirc_hit->get_z(1);
      mG4EvtTree.edep[nhits] = dirc_hit->get_edep();

      mG4EvtTree.mcp_id[nhits] = dirc_hit->GetMcpId();
      mG4EvtTree.pixel_id[nhits] = dirc_hit->GetPixelId();
      mG4EvtTree.CherenkovMC[nhits] = dirc_hit->GetCherenkovMC();
      mG4EvtTree.lead_time[nhits] = dirc_hit->GetLeadTime();
      mG4EvtTree.wavelength[nhits] = dirc_hit->GetTotTime();
      mG4EvtTree.hit_pathId[nhits] = dirc_hit->GetPathInPrizm();

      for(int i=0; i < 3; i++)
        {
          mG4EvtTree.hit_globalPos[nhits][i] = dirc_hit->GetGlobalPos()(i);
          mG4EvtTree.hit_localPos[nhits][i] = dirc_hit->GetLocalPos()(i);
          mG4EvtTree.hit_digiPos[nhits][i] = dirc_hit->GetDigiPos()(i);
          mG4EvtTree.hit_mom[nhits][i] = dirc_hit->GetMomentum()(i);
          mG4EvtTree.hit_pos[nhits][i] = dirc_hit->GetPosition()(i);
	}

      /*layer_edep_map[hit_iter->second->get_layer()] += hit_iter->second->get_edep();
      mG4EvtTree.detid[nhits] = detid;
      mG4EvtTree.layer[nhits] = hit_iter->second->get_layer();
      mG4EvtTree.hitid[nhits] = hit_iter->second->get_hit_id();
      mG4EvtTree.trkid[nhits] = hit_iter->second->get_trkid();
      mG4EvtTree.scintid[nhits] = hit_iter->second->get_scint_id();
      mG4EvtTree.x0[nhits] = hit_iter->second->get_x(0);
      mG4EvtTree.y0[nhits] = hit_iter->second->get_y(0);
      mG4EvtTree.z0[nhits] = hit_iter->second->get_z(0);
      mG4EvtTree.x1[nhits] = hit_iter->second->get_x(1);
      mG4EvtTree.y1[nhits] = hit_iter->second->get_y(1);
      mG4EvtTree.z1[nhits] = hit_iter->second->get_z(1);
      mG4EvtTree.edep[nhits] = hit_iter->second->get_edep();

      mG4EvtTree.mcp_id[nhits] = hit_iter->second->GetMcpId();
      mG4EvtTree.pixel_id[nhits] = hit_iter->second->GetPixelId();
      mG4EvtTree.CherenkovMC[nhits] = hit_iter->second->GetCherenkovMC();
      mG4EvtTree.lead_time[nhits] = hit_iter->second->GetLeadTime();
      mG4EvtTree.wavelength[nhits] = hit_iter->second->GetTotTime();

      for(int i=0; i < 3; i++)
	{
	  mG4EvtTree.hit_globalPos[nhits][i] = hit_iter->second->GetGlobalPos()(i);
	  mG4EvtTree.hit_localPos[nhits][i] = hit_iter->second->GetLocalPos()(i);
	  mG4EvtTree.hit_digiPos[nhits][i] = hit_iter->second->GetDigiPos()(i);
	  mG4EvtTree.hit_mom[nhits][i] = hit_iter->second->GetMomentum()(i);
	  mG4EvtTree.hit_pos[nhits][i] = hit_iter->second->GetPosition()(i);
	  }*/

      nhits++;
      // esum += hit_iter->second->get_edep();
    }
    for (edepiter = layer_edep_map.begin(); edepiter != layer_edep_map.end(); ++edepiter)
    {
      nLayers = edepiter->first - 1;
      if (!strcmp("G4HIT_CEMC", dName.c_str()))
        mG4EvtTree.cemcactESum[nLayers] = edepiter->second;
      else if (!strcmp("G4HIT_ABSORBER_CEMC", dName.c_str()))
        mG4EvtTree.cemcabsESum[nLayers] = edepiter->second;
      else if (!strcmp("G4HIT_HCAL", dName.c_str()))
        mG4EvtTree.hcalactESum[nLayers] = edepiter->second;
      else if (!strcmp("G4HIT_ABSORBER_HCAL", dName.c_str()))
        mG4EvtTree.hcalabsESum[nLayers] = edepiter->second;
    }
  }

  return nLayers + 1;
}
