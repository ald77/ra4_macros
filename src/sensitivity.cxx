#include "sensitivity.hpp"

#include <sstream>
#include <vector>

#include <unistd.h>
#include <getopt.h>

#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGaxis.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"

#include "styles.hpp"

using namespace std;

namespace{
  enum UncertSet{
    kNormal, kNoSyst, kContaminated
  };
  UncertSet uncert_set = kNormal;
  int method = 1;
}

int main(int argc, char *argv[]){
  GetOptions(argc, argv);
  styles style("LargeLabels");
  style.setDefaultStyle();
  gStyle->SetPadTickX(1);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);

  vector<double> m1_signif_nc, m1_limit_nc;
  vector<double> m1_signif_c, m1_limit_c;
  vector<double> m3_signif_nc, m3_limit_nc;
  vector<double> m3_signif_c, m3_limit_c;
  vector<double> x(0), ncy(0), cy(0), ncz(0), cz(0);

  switch(uncert_set){
  case kNormal:
    m1_signif_nc.push_back(0.517548); m1_limit_nc.push_back(1.316410); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.671903); m1_limit_nc.push_back(0.658203); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.784878); m1_limit_nc.push_back(0.439453); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.868115); m1_limit_nc.push_back(0.329102); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.932990); m1_limit_nc.push_back(0.263672); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.985406); m1_limit_nc.push_back(0.219727); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.595437); m3_limit_nc.push_back(1.316410); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.795439); m3_limit_nc.push_back(0.658203); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.939719); m3_limit_nc.push_back(0.439453); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.051140); m3_limit_nc.push_back(0.329102); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.142400); m3_limit_nc.push_back(0.263672); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.220060); m3_limit_nc.push_back(0.219727); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    break;
  case kNoSyst:
    m1_signif_nc.push_back(0.563099); m1_limit_nc.push_back(1.316410); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.767605); m1_limit_nc.push_back(0.658203); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.940119); m1_limit_nc.push_back(0.439453); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.085560); m1_limit_nc.push_back(0.329102); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.213690); m1_limit_nc.push_back(0.263672); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.329530); m1_limit_nc.push_back(0.219727); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.660924); m3_limit_nc.push_back(1.316410); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.934687); m3_limit_nc.push_back(0.658203); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.144750); m3_limit_nc.push_back(0.439453); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.321850); m3_limit_nc.push_back(0.329102); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.477870); m3_limit_nc.push_back(0.263672); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.618920); m3_limit_nc.push_back(0.219727); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    break;
  case kContaminated:
    m1_signif_nc.push_back(0.651089); m1_limit_nc.push_back(2.835940); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(0.884065); m1_limit_nc.push_back(1.417970); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.033470); m1_limit_nc.push_back(0.949219); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.143830); m1_limit_nc.push_back(0.708984); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.230040); m1_limit_nc.push_back(0.568359); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m1_signif_nc.push_back(1.299850); m1_limit_nc.push_back(0.474609); m1_signif_c.push_back(0.); m1_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.660744); m3_limit_nc.push_back(1.835940); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(0.885141); m3_limit_nc.push_back(0.917969); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.048840); m3_limit_nc.push_back(0.611328); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.176520); m3_limit_nc.push_back(0.458984); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.282110); m3_limit_nc.push_back(0.367188); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    m3_signif_nc.push_back(1.372740); m3_limit_nc.push_back(0.305664); m3_signif_c.push_back(0.); m3_limit_c.push_back(0.);
    break;
  default: break;
  }

  m1_signif_c = m1_signif_nc;
  m1_limit_c = m1_limit_nc;
  m3_signif_c = m3_signif_nc;
  m3_limit_c = m3_limit_nc;

  switch(method){
  case 1:
    ncy = m1_signif_nc;
    cy = m1_signif_c;
    ncz = m1_limit_nc;
    cz = m1_limit_c;
    break;
  case 3:
    ncy = m3_signif_nc;
    cy = m3_signif_c;
    ncz = m3_limit_nc;
    cz = m3_limit_c;
    break;
  default:
    break;
  }

  double big_num = 10.;
  x.clear();
  for(size_t i = 0; i < ncy.size(); ++i){
    x.push_back((i+1.)*0.5);
    if(ncz.at(i)<=0.){
      ncz.at(i) = big_num;
    }
    ncy.at(i) *= 1.;
    if(cz.at(i) <= 0.){
      cz.at(i) = big_num;
    }
    cy.at(i) *= 1.;
  }

  x.insert(x.begin(), 0.);
  ncy.insert(ncy.begin(), 0.);
  cy.insert(cy.begin(), 0.);
  ncz.insert(ncz.begin(), 0.);
  cz.insert(cz.begin(), 0.);

  TGraph snc(x.size(), &x.at(0), &ncy.at(0));
  TGraph sc(x.size(), &x.at(0), &cy.at(0));
  TGraph lnc(x.size()-1, &x.at(1), &ncz.at(1));
  TGraph lc(x.size()-1, &x.at(1), &cz.at(1));
  snc.SetLineStyle(1);
  snc.SetLineWidth(5);
  TGraph dnc = snc;
  snc.SetLineColor(kRed);
  sc.SetLineStyle(2);
  sc.SetLineWidth(5);
  TGraph dc = sc;
  sc.SetLineColor(kRed);
  lnc.SetLineColor(kBlue);
  lnc.SetLineStyle(1);
  lnc.SetLineWidth(5);
  lc.SetLineColor(kBlue);
  lc.SetLineStyle(2);
  lc.SetLineWidth(5);
  TH1D h("h", ";Luminosity [fb^{-1}];Expected Limit/X-Section", 1, 0., 3.);
  h.SetMaximum(2.);
  TAxis &yaxis = *h.GetYaxis();
  yaxis.SetLabelColor(kBlue);
  yaxis.SetTitleColor(kBlue);
  yaxis.SetTitleOffset(.8);
  TCanvas c;
  c.SetTicks(1,0);
  TGaxis *raxis = new TGaxis(3., 0.,
                             3., 2.,
                             0, 2., 510, "+L");
  raxis->SetLabelColor(kRed);
  raxis->SetTitleColor(kRed);
  raxis->SetTitle("Expected Significance");
  raxis->SetTitleOffset(yaxis.GetTitleOffset());
  raxis->SetTitleSize(yaxis.GetTitleSize());
  //raxis->SetTextSize(yaxis.GetTextSize());
  raxis->SetLabelSize(yaxis.GetLabelSize());
  raxis->SetLabelFont(yaxis.GetLabelFont());
  raxis->SetTitleFont(yaxis.GetTitleFont());

  h.Draw("hist");
  snc.Draw("samel");
  sc.Draw("samel");
  lnc.Draw("samel");
  lc.Draw("samel");
  raxis->Draw("same");
  TLegend l(1.0-gStyle->GetPadRightMargin()-0.4, 1.0-gStyle->GetPadTopMargin()-0.25,
	    1.0-gStyle->GetPadRightMargin(), 1.0-gStyle->GetPadTopMargin());
  l.SetBorderSize(0);
  l.SetFillColor(0);
  l.SetFillStyle(4000);
  l.AddEntry(&dnc, "T1tttt(1500,100)", "l");
  l.AddEntry(&dc, "T1tttt(1200,800)", "l");
  l.Draw("same");

  ostringstream oss;
  oss << "plots/sensitivity";
  switch(uncert_set){
  case kNormal: oss << "_normal"; break;
  case kNoSyst: oss << "_nosyst"; break;
  case kContaminated: oss << "_contam"; break;
  default: break;
  }
  oss << "_method_" << method << ".pdf" << flush;
  c.Print(oss.str().c_str());
}

void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"uncerts", required_argument, 0, 'u'},
      {"method", required_argument, 0, 'm'},
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "u:m:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'u':
      switch(atoi(optarg)){
      case 0: uncert_set = kNormal; break;
      case 1: uncert_set = kNoSyst; break;
      case 2: uncert_set = kContaminated; break;
      default: break;
      }
      break;
    case 'm':
      method = atoi(optarg);
      break;
    case 0:
      optname = long_options[option_index].name;
      break;
    default: break;
    }
  }
}
