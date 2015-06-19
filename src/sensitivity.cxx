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
    kAll, kStatOnly, kSystOnly
  };
  UncertSet uncert_set = kAll;
  int method = 1;
}

int main(int argc, char *argv[]){
  GetOptions(argc, argv);
  styles style("LargeLabels");
  style.setDefaultStyle();
  gStyle->SetPadTickX(1);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.12);

  vector<double> m1_signif_T1tttt_1500_100, m1_limit_T1tttt_1500_100;
  vector<double> m1_signif_T1tttt_1200_800, m1_limit_T1tttt_1200_800;
  vector<double> m2_signif_T1tttt_1500_100, m2_limit_T1tttt_1500_100;
  vector<double> m2_signif_T1tttt_1200_800, m2_limit_T1tttt_1200_800;
  vector<double> m3_signif_T1tttt_1500_100, m3_limit_T1tttt_1500_100;
  vector<double> m3_signif_T1tttt_1200_800, m3_limit_T1tttt_1200_800;
  vector<double> x(0), ncy(0), cy(0), ncz(0), cz(0);

  switch(uncert_set){
  case kAll:
    m1_signif_T1tttt_1500_100.push_back(1.5015); m1_limit_T1tttt_1500_100.push_back(1.77344); m1_signif_T1tttt_1200_800.push_back(1.13612); m1_limit_T1tttt_1200_800.push_back(3.92188);
    m1_signif_T1tttt_1500_100.push_back(1.77509); m1_limit_T1tttt_1500_100.push_back(0.988281); m1_signif_T1tttt_1200_800.push_back(1.30736); m1_limit_T1tttt_1200_800.push_back(2.22656);
    m1_signif_T1tttt_1500_100.push_back(1.95533); m1_limit_T1tttt_1500_100.push_back(0.705078); m1_signif_T1tttt_1200_800.push_back(1.40613); m1_limit_T1tttt_1200_800.push_back(1.61719);
    m1_signif_T1tttt_1500_100.push_back(2.08047); m1_limit_T1tttt_1500_100.push_back(0.564453); m1_signif_T1tttt_1200_800.push_back(1.47649); m1_limit_T1tttt_1200_800.push_back(1.29297);
    m1_signif_T1tttt_1500_100.push_back(2.1731); m1_limit_T1tttt_1500_100.push_back(0.476562); m1_signif_T1tttt_1200_800.push_back(1.53014); m1_limit_T1tttt_1200_800.push_back(1.09766);
    m1_signif_T1tttt_1500_100.push_back(2.24685); m1_limit_T1tttt_1500_100.push_back(0.419922); m1_signif_T1tttt_1200_800.push_back(1.57269); m1_limit_T1tttt_1200_800.push_back(0.964844);
    m1_signif_T1tttt_1500_100.push_back(2.30771); m1_limit_T1tttt_1500_100.push_back(0.375977); m1_signif_T1tttt_1200_800.push_back(1.60774); m1_limit_T1tttt_1200_800.push_back(0.863281);
    m1_signif_T1tttt_1500_100.push_back(2.35889); m1_limit_T1tttt_1500_100.push_back(0.344727); m1_signif_T1tttt_1200_800.push_back(1.63751); m1_limit_T1tttt_1200_800.push_back(0.792969);
    m1_signif_T1tttt_1500_100.push_back(2.40329); m1_limit_T1tttt_1500_100.push_back(0.319336); m1_signif_T1tttt_1200_800.push_back(1.66301); m1_limit_T1tttt_1200_800.push_back(0.732422);
    m1_signif_T1tttt_1500_100.push_back(2.44199); m1_limit_T1tttt_1500_100.push_back(0.298828); m1_signif_T1tttt_1200_800.push_back(1.68547); m1_limit_T1tttt_1200_800.push_back(0.681641);
    m1_signif_T1tttt_1500_100.push_back(2.47595); m1_limit_T1tttt_1500_100.push_back(0.282227); m1_signif_T1tttt_1200_800.push_back(1.70557); m1_limit_T1tttt_1200_800.push_back(0.642578);
    m1_signif_T1tttt_1500_100.push_back(2.50588); m1_limit_T1tttt_1500_100.push_back(0.268555); m1_signif_T1tttt_1200_800.push_back(1.72351); m1_limit_T1tttt_1200_800.push_back(0.607422);
    m1_signif_T1tttt_1500_100.push_back(2.53301); m1_limit_T1tttt_1500_100.push_back(0.254883); m1_signif_T1tttt_1200_800.push_back(1.73959); m1_limit_T1tttt_1200_800.push_back(0.580078);
    m1_signif_T1tttt_1500_100.push_back(2.5575); m1_limit_T1tttt_1500_100.push_back(0.245117); m1_signif_T1tttt_1200_800.push_back(1.75431); m1_limit_T1tttt_1200_800.push_back(0.552734);
    m1_signif_T1tttt_1500_100.push_back(2.57973); m1_limit_T1tttt_1500_100.push_back(0.235352); m1_signif_T1tttt_1200_800.push_back(1.7676); m1_limit_T1tttt_1200_800.push_back(0.533203);
    m1_signif_T1tttt_1500_100.push_back(2.59962); m1_limit_T1tttt_1500_100.push_back(0.227539); m1_signif_T1tttt_1200_800.push_back(1.7798); m1_limit_T1tttt_1200_800.push_back(0.513672);
    m1_signif_T1tttt_1500_100.push_back(2.61826); m1_limit_T1tttt_1500_100.push_back(0.219727); m1_signif_T1tttt_1200_800.push_back(1.79118); m1_limit_T1tttt_1200_800.push_back(0.494141);
    m1_signif_T1tttt_1500_100.push_back(2.63496); m1_limit_T1tttt_1500_100.push_back(0.213867); m1_signif_T1tttt_1200_800.push_back(1.80161); m1_limit_T1tttt_1200_800.push_back(0.478516);
    m1_signif_T1tttt_1500_100.push_back(2.6504); m1_limit_T1tttt_1500_100.push_back(0.208008); m1_signif_T1tttt_1200_800.push_back(1.81113); m1_limit_T1tttt_1200_800.push_back(0.462891);
    m1_signif_T1tttt_1500_100.push_back(2.66486); m1_limit_T1tttt_1500_100.push_back(0.202148); m1_signif_T1tttt_1200_800.push_back(1.81978); m1_limit_T1tttt_1200_800.push_back(0.451172);
    m2_signif_T1tttt_1500_100.push_back(1.32192); m2_limit_T1tttt_1500_100.push_back(1.60156); m2_signif_T1tttt_1200_800.push_back(1.28962); m2_limit_T1tttt_1200_800.push_back(1.97656);
    m2_signif_T1tttt_1500_100.push_back(1.73585); m2_limit_T1tttt_1500_100.push_back(0.9375); m2_signif_T1tttt_1200_800.push_back(1.70612); m2_limit_T1tttt_1200_800.push_back(1.20312);
    m2_signif_T1tttt_1500_100.push_back(2.01333); m2_limit_T1tttt_1500_100.push_back(0.697266); m2_signif_T1tttt_1200_800.push_back(1.98216); m2_limit_T1tttt_1200_800.push_back(0.910156);
    m2_signif_T1tttt_1500_100.push_back(2.20842); m2_limit_T1tttt_1500_100.push_back(0.572266); m2_signif_T1tttt_1200_800.push_back(2.17574); m2_limit_T1tttt_1200_800.push_back(0.748047);
    m2_signif_T1tttt_1500_100.push_back(2.345); m2_limit_T1tttt_1500_100.push_back(0.490234); m2_signif_T1tttt_1200_800.push_back(2.31977); m2_limit_T1tttt_1200_800.push_back(0.642578);
    m2_signif_T1tttt_1500_100.push_back(2.44612); m2_limit_T1tttt_1500_100.push_back(0.435547); m2_signif_T1tttt_1200_800.push_back(2.43309); m2_limit_T1tttt_1200_800.push_back(0.572266);
    m2_signif_T1tttt_1500_100.push_back(2.52766); m2_limit_T1tttt_1500_100.push_back(0.392578); m2_signif_T1tttt_1200_800.push_back(2.52689); m2_limit_T1tttt_1200_800.push_back(0.517578);
    m2_signif_T1tttt_1500_100.push_back(2.59634); m2_limit_T1tttt_1500_100.push_back(0.362305); m2_signif_T1tttt_1200_800.push_back(2.60653); m2_limit_T1tttt_1200_800.push_back(0.474609);
    m2_signif_T1tttt_1500_100.push_back(2.65362); m2_limit_T1tttt_1500_100.push_back(0.336914); m2_signif_T1tttt_1200_800.push_back(2.67494); m2_limit_T1tttt_1200_800.push_back(0.439453);
    m2_signif_T1tttt_1500_100.push_back(2.70324); m2_limit_T1tttt_1500_100.push_back(0.317383); m2_signif_T1tttt_1200_800.push_back(2.73497); m2_limit_T1tttt_1200_800.push_back(0.412109);
    m2_signif_T1tttt_1500_100.push_back(2.74985); m2_limit_T1tttt_1500_100.push_back(0.299805); m2_signif_T1tttt_1200_800.push_back(2.78932); m2_limit_T1tttt_1200_800.push_back(0.390625);
    m2_signif_T1tttt_1500_100.push_back(2.79006); m2_limit_T1tttt_1500_100.push_back(0.28418); m2_signif_T1tttt_1200_800.push_back(2.83729); m2_limit_T1tttt_1200_800.push_back(0.370117);
    m2_signif_T1tttt_1500_100.push_back(2.82679); m2_limit_T1tttt_1500_100.push_back(0.272461); m2_signif_T1tttt_1200_800.push_back(2.88064); m2_limit_T1tttt_1200_800.push_back(0.352539);
    m2_signif_T1tttt_1500_100.push_back(2.85953); m2_limit_T1tttt_1500_100.push_back(0.260742); m2_signif_T1tttt_1200_800.push_back(2.91976); m2_limit_T1tttt_1200_800.push_back(0.338867);
    m2_signif_T1tttt_1500_100.push_back(2.88888); m2_limit_T1tttt_1500_100.push_back(0.250977); m2_signif_T1tttt_1200_800.push_back(2.95536); m2_limit_T1tttt_1200_800.push_back(0.325195);
    m2_signif_T1tttt_1500_100.push_back(2.91672); m2_limit_T1tttt_1500_100.push_back(0.243164); m2_signif_T1tttt_1200_800.push_back(2.98824); m2_limit_T1tttt_1200_800.push_back(0.313477);
    m2_signif_T1tttt_1500_100.push_back(2.94001); m2_limit_T1tttt_1500_100.push_back(0.235352); m2_signif_T1tttt_1200_800.push_back(3.0179); m2_limit_T1tttt_1200_800.push_back(0.303711);
    m2_signif_T1tttt_1500_100.push_back(2.96225); m2_limit_T1tttt_1500_100.push_back(0.227539); m2_signif_T1tttt_1200_800.push_back(3.04533); m2_limit_T1tttt_1200_800.push_back(0.293945);
    m2_signif_T1tttt_1500_100.push_back(2.98361); m2_limit_T1tttt_1500_100.push_back(0.22168); m2_signif_T1tttt_1200_800.push_back(3.07121); m2_limit_T1tttt_1200_800.push_back(0.286133);
    m2_signif_T1tttt_1500_100.push_back(3.00339); m2_limit_T1tttt_1500_100.push_back(0.21582); m2_signif_T1tttt_1200_800.push_back(3.09534); m2_limit_T1tttt_1200_800.push_back(0.27832);
    m3_signif_T1tttt_1500_100.push_back(1.39687); m3_limit_T1tttt_1500_100.push_back(1.51172); m3_signif_T1tttt_1200_800.push_back(1.52212); m3_limit_T1tttt_1200_800.push_back(1.72656);
    m3_signif_T1tttt_1500_100.push_back(1.8424); m3_limit_T1tttt_1500_100.push_back(0.894531); m3_signif_T1tttt_1200_800.push_back(2.02902); m3_limit_T1tttt_1200_800.push_back(1.01953);
    m3_signif_T1tttt_1500_100.push_back(2.14312); m3_limit_T1tttt_1500_100.push_back(0.669922); m3_signif_T1tttt_1200_800.push_back(2.36543); m3_limit_T1tttt_1200_800.push_back(0.755859);
    m3_signif_T1tttt_1500_100.push_back(2.358); m3_limit_T1tttt_1500_100.push_back(0.548828); m3_signif_T1tttt_1200_800.push_back(2.6076); m3_limit_T1tttt_1200_800.push_back(0.615234);
    m3_signif_T1tttt_1500_100.push_back(2.51266); m3_limit_T1tttt_1500_100.push_back(0.474609); m3_signif_T1tttt_1200_800.push_back(2.7921); m3_limit_T1tttt_1200_800.push_back(0.529297);
    m3_signif_T1tttt_1500_100.push_back(2.63048); m3_limit_T1tttt_1500_100.push_back(0.419922); m3_signif_T1tttt_1200_800.push_back(2.93995); m3_limit_T1tttt_1200_800.push_back(0.466797);
    m3_signif_T1tttt_1500_100.push_back(2.7271); m3_limit_T1tttt_1500_100.push_back(0.381836); m3_signif_T1tttt_1200_800.push_back(3.06357); m3_limit_T1tttt_1200_800.push_back(0.423828);
    m3_signif_T1tttt_1500_100.push_back(2.80935); m3_limit_T1tttt_1500_100.push_back(0.350586); m3_signif_T1tttt_1200_800.push_back(3.16884); m3_limit_T1tttt_1200_800.push_back(0.389648);
    m3_signif_T1tttt_1500_100.push_back(2.87908); m3_limit_T1tttt_1500_100.push_back(0.327148); m3_signif_T1tttt_1200_800.push_back(3.25997); m3_limit_T1tttt_1200_800.push_back(0.362305);
    m3_signif_T1tttt_1500_100.push_back(2.94009); m3_limit_T1tttt_1500_100.push_back(0.307617); m3_signif_T1tttt_1200_800.push_back(3.34006); m3_limit_T1tttt_1200_800.push_back(0.338867);
    m3_signif_T1tttt_1500_100.push_back(2.99682); m3_limit_T1tttt_1500_100.push_back(0.290039); m3_signif_T1tttt_1200_800.push_back(3.41181); m3_limit_T1tttt_1200_800.push_back(0.321289);
    m3_signif_T1tttt_1500_100.push_back(3.04643); m3_limit_T1tttt_1500_100.push_back(0.276367); m3_signif_T1tttt_1200_800.push_back(3.47562); m3_limit_T1tttt_1200_800.push_back(0.305664);
    m3_signif_T1tttt_1500_100.push_back(3.09177); m3_limit_T1tttt_1500_100.push_back(0.264648); m3_signif_T1tttt_1200_800.push_back(3.53331); m3_limit_T1tttt_1200_800.push_back(0.291992);
    m3_signif_T1tttt_1500_100.push_back(3.13247); m3_limit_T1tttt_1500_100.push_back(0.254883); m3_signif_T1tttt_1200_800.push_back(3.5854); m3_limit_T1tttt_1200_800.push_back(0.280273);
    m3_signif_T1tttt_1500_100.push_back(3.16939); m3_limit_T1tttt_1500_100.push_back(0.245117); m3_signif_T1tttt_1200_800.push_back(3.63319); m3_limit_T1tttt_1200_800.push_back(0.268555);
    m3_signif_T1tttt_1500_100.push_back(3.2042); m3_limit_T1tttt_1500_100.push_back(0.237305); m3_signif_T1tttt_1200_800.push_back(3.67714); m3_limit_T1tttt_1200_800.push_back(0.260742);
    m3_signif_T1tttt_1500_100.push_back(3.23422); m3_limit_T1tttt_1500_100.push_back(0.229492); m3_signif_T1tttt_1200_800.push_back(3.71708); m3_limit_T1tttt_1200_800.push_back(0.25293);
    m3_signif_T1tttt_1500_100.push_back(3.26273); m3_limit_T1tttt_1500_100.push_back(0.223633); m3_signif_T1tttt_1200_800.push_back(3.75406); m3_limit_T1tttt_1200_800.push_back(0.245117);
    m3_signif_T1tttt_1500_100.push_back(3.28984); m3_limit_T1tttt_1500_100.push_back(0.217773); m3_signif_T1tttt_1200_800.push_back(3.78865); m3_limit_T1tttt_1200_800.push_back(0.238281);
    m3_signif_T1tttt_1500_100.push_back(3.31504); m3_limit_T1tttt_1500_100.push_back(0.211914); m3_signif_T1tttt_1200_800.push_back(3.82088); m3_limit_T1tttt_1200_800.push_back(0.231445);
    break;
  case kStatOnly:
    m1_signif_T1tttt_1500_100.push_back(1.6895); m1_limit_T1tttt_1500_100.push_back(1.71094); m1_signif_T1tttt_1200_800.push_back(1.24604); m1_limit_T1tttt_1200_800.push_back(3.76562); 
    m1_signif_T1tttt_1500_100.push_back(2.047); m1_limit_T1tttt_1500_100.push_back(0.941406); m1_signif_T1tttt_1200_800.push_back(1.4649); m1_limit_T1tttt_1200_800.push_back(2.11719); 
    m1_signif_T1tttt_1500_100.push_back(2.29396); m1_limit_T1tttt_1500_100.push_back(0.673828); m1_signif_T1tttt_1200_800.push_back(1.59816); m1_limit_T1tttt_1200_800.push_back(1.51953); 
    m1_signif_T1tttt_1500_100.push_back(2.47263); m1_limit_T1tttt_1500_100.push_back(0.537109); m1_signif_T1tttt_1200_800.push_back(1.69693); m1_limit_T1tttt_1200_800.push_back(1.21484); 
    m1_signif_T1tttt_1500_100.push_back(2.61037); m1_limit_T1tttt_1500_100.push_back(0.455078); m1_signif_T1tttt_1200_800.push_back(1.77473); m1_limit_T1tttt_1200_800.push_back(1.02734); 
    m1_signif_T1tttt_1500_100.push_back(2.72372); m1_limit_T1tttt_1500_100.push_back(0.400391); m1_signif_T1tttt_1200_800.push_back(1.83799); m1_limit_T1tttt_1200_800.push_back(0.902344); 
    m1_signif_T1tttt_1500_100.push_back(2.82023); m1_limit_T1tttt_1500_100.push_back(0.358398); m1_signif_T1tttt_1200_800.push_back(1.89147); m1_limit_T1tttt_1200_800.push_back(0.808594); 
    m1_signif_T1tttt_1500_100.push_back(2.90369); m1_limit_T1tttt_1500_100.push_back(0.327148); m1_signif_T1tttt_1200_800.push_back(1.93801); m1_limit_T1tttt_1200_800.push_back(0.740234); 
    m1_signif_T1tttt_1500_100.push_back(2.97785); m1_limit_T1tttt_1500_100.push_back(0.303711); m1_signif_T1tttt_1200_800.push_back(1.97854); m1_limit_T1tttt_1200_800.push_back(0.685547); 
    m1_signif_T1tttt_1500_100.push_back(3.04379); m1_limit_T1tttt_1500_100.push_back(0.28418); m1_signif_T1tttt_1200_800.push_back(2.01482); m1_limit_T1tttt_1200_800.push_back(0.642578); 
    m1_signif_T1tttt_1500_100.push_back(3.1028); m1_limit_T1tttt_1500_100.push_back(0.268555); m1_signif_T1tttt_1200_800.push_back(2.04785); m1_limit_T1tttt_1200_800.push_back(0.603516); 
    m1_signif_T1tttt_1500_100.push_back(3.15525); m1_limit_T1tttt_1500_100.push_back(0.254883); m1_signif_T1tttt_1200_800.push_back(2.07726); m1_limit_T1tttt_1200_800.push_back(0.572266); 
    m1_signif_T1tttt_1500_100.push_back(3.2038); m1_limit_T1tttt_1500_100.push_back(0.243164); m1_signif_T1tttt_1200_800.push_back(2.10425); m1_limit_T1tttt_1200_800.push_back(0.544922); 
    m1_signif_T1tttt_1500_100.push_back(3.24823); m1_limit_T1tttt_1500_100.push_back(0.233398); m1_signif_T1tttt_1200_800.push_back(2.12916); m1_limit_T1tttt_1200_800.push_back(0.521484); 
    m1_signif_T1tttt_1500_100.push_back(3.28936); m1_limit_T1tttt_1500_100.push_back(0.224609); m1_signif_T1tttt_1200_800.push_back(2.15222); m1_limit_T1tttt_1200_800.push_back(0.501953); 
    m1_signif_T1tttt_1500_100.push_back(3.32635); m1_limit_T1tttt_1500_100.push_back(0.217773); m1_signif_T1tttt_1200_800.push_back(2.1733); m1_limit_T1tttt_1200_800.push_back(0.484375); 
    m1_signif_T1tttt_1500_100.push_back(3.3617); m1_limit_T1tttt_1500_100.push_back(0.209961); m1_signif_T1tttt_1200_800.push_back(2.19344); m1_limit_T1tttt_1200_800.push_back(0.466797); 
    m1_signif_T1tttt_1500_100.push_back(3.39343); m1_limit_T1tttt_1500_100.push_back(0.204102); m1_signif_T1tttt_1200_800.push_back(2.21174); m1_limit_T1tttt_1200_800.push_back(0.455078); 
    m1_signif_T1tttt_1500_100.push_back(3.42321); m1_limit_T1tttt_1500_100.push_back(0.198242); m1_signif_T1tttt_1200_800.push_back(2.22866); m1_limit_T1tttt_1200_800.push_back(0.439453); 
    m1_signif_T1tttt_1500_100.push_back(3.45155); m1_limit_T1tttt_1500_100.push_back(0.193359); m1_signif_T1tttt_1200_800.push_back(2.2443); m1_limit_T1tttt_1200_800.push_back(0.427734); 
    m2_signif_T1tttt_1500_100.push_back(1.43908); m2_limit_T1tttt_1500_100.push_back(1.55078); m2_signif_T1tttt_1200_800.push_back(1.44496); m2_limit_T1tttt_1200_800.push_back(1.86719); 
    m2_signif_T1tttt_1500_100.push_back(1.92993); m2_limit_T1tttt_1500_100.push_back(0.910156); m2_signif_T1tttt_1200_800.push_back(1.95502); m2_limit_T1tttt_1200_800.push_back(1.12891); 
    m2_signif_T1tttt_1500_100.push_back(2.27987); m2_limit_T1tttt_1500_100.push_back(0.677734); m2_signif_T1tttt_1200_800.push_back(2.31213); m2_limit_T1tttt_1200_800.push_back(0.847656); 
    m2_signif_T1tttt_1500_100.push_back(2.53921); m2_limit_T1tttt_1500_100.push_back(0.552734); m2_signif_T1tttt_1200_800.push_back(2.57716); m2_limit_T1tttt_1200_800.push_back(0.693359); 
    m2_signif_T1tttt_1500_100.push_back(2.72735); m2_limit_T1tttt_1500_100.push_back(0.474609); m2_signif_T1tttt_1200_800.push_back(2.78022); m2_limit_T1tttt_1200_800.push_back(0.595703); 
    m2_signif_T1tttt_1500_100.push_back(2.87039); m2_limit_T1tttt_1500_100.push_back(0.419922); m2_signif_T1tttt_1200_800.push_back(2.94293); m2_limit_T1tttt_1200_800.push_back(0.529297); 
    m2_signif_T1tttt_1500_100.push_back(2.98872); m2_limit_T1tttt_1500_100.push_back(0.37793); m2_signif_T1tttt_1200_800.push_back(3.08073); m2_limit_T1tttt_1200_800.push_back(0.478516); 
    m2_signif_T1tttt_1500_100.push_back(3.09059); m2_limit_T1tttt_1500_100.push_back(0.347656); m2_signif_T1tttt_1200_800.push_back(3.19958); m2_limit_T1tttt_1200_800.push_back(0.439453); 
    m2_signif_T1tttt_1500_100.push_back(3.17709); m2_limit_T1tttt_1500_100.push_back(0.323242); m2_signif_T1tttt_1200_800.push_back(3.30303); m2_limit_T1tttt_1200_800.push_back(0.408203); 
    m2_signif_T1tttt_1500_100.push_back(3.25361); m2_limit_T1tttt_1500_100.push_back(0.301758); m2_signif_T1tttt_1200_800.push_back(3.39596); m2_limit_T1tttt_1200_800.push_back(0.381836); 
    m2_signif_T1tttt_1500_100.push_back(3.3269); m2_limit_T1tttt_1500_100.push_back(0.286133); m2_signif_T1tttt_1200_800.push_back(3.48172); m2_limit_T1tttt_1200_800.push_back(0.360352); 
    m2_signif_T1tttt_1500_100.push_back(3.3911); m2_limit_T1tttt_1500_100.push_back(0.271484); m2_signif_T1tttt_1200_800.push_back(3.55834); m2_limit_T1tttt_1200_800.push_back(0.34082); 
    m2_signif_T1tttt_1500_100.push_back(3.45068); m2_limit_T1tttt_1500_100.push_back(0.258789); m2_signif_T1tttt_1200_800.push_back(3.62868); m2_limit_T1tttt_1200_800.push_back(0.325195); 
    m2_signif_T1tttt_1500_100.push_back(3.50432); m2_limit_T1tttt_1500_100.push_back(0.249023); m2_signif_T1tttt_1200_800.push_back(3.69233); m2_limit_T1tttt_1200_800.push_back(0.311523); 
    m2_signif_T1tttt_1500_100.push_back(3.55303); m2_limit_T1tttt_1500_100.push_back(0.239258); m2_signif_T1tttt_1200_800.push_back(3.75099); m2_limit_T1tttt_1200_800.push_back(0.299805); 
    m2_signif_T1tttt_1500_100.push_back(3.59983); m2_limit_T1tttt_1500_100.push_back(0.231445); m2_signif_T1tttt_1200_800.push_back(3.80592); m2_limit_T1tttt_1200_800.push_back(0.290039); 
    m2_signif_T1tttt_1500_100.push_back(3.63914); m2_limit_T1tttt_1500_100.push_back(0.223633); m2_signif_T1tttt_1200_800.push_back(3.8551); m2_limit_T1tttt_1200_800.push_back(0.280273); 
    m2_signif_T1tttt_1500_100.push_back(3.67715); m2_limit_T1tttt_1500_100.push_back(0.21582); m2_signif_T1tttt_1200_800.push_back(3.90129); m2_limit_T1tttt_1200_800.push_back(0.270508); 
    m2_signif_T1tttt_1500_100.push_back(3.71391); m2_limit_T1tttt_1500_100.push_back(0.209961); m2_signif_T1tttt_1200_800.push_back(3.94504); m2_limit_T1tttt_1200_800.push_back(0.262695); 
    m2_signif_T1tttt_1500_100.push_back(3.74838); m2_limit_T1tttt_1500_100.push_back(0.204102); m2_signif_T1tttt_1200_800.push_back(3.98648); m2_limit_T1tttt_1200_800.push_back(0.256836); 
    m3_signif_T1tttt_1500_100.push_back(1.51549); m3_limit_T1tttt_1500_100.push_back(1.48047); m3_signif_T1tttt_1200_800.push_back(1.66406); m3_limit_T1tttt_1200_800.push_back(1.66406); 
    m3_signif_T1tttt_1500_100.push_back(2.0393); m3_limit_T1tttt_1500_100.push_back(0.871094); m3_signif_T1tttt_1200_800.push_back(2.26046); m3_limit_T1tttt_1200_800.push_back(0.980469); 
    m3_signif_T1tttt_1500_100.push_back(2.41065); m3_limit_T1tttt_1500_100.push_back(0.650391); m3_signif_T1tttt_1200_800.push_back(2.66735); m3_limit_T1tttt_1200_800.push_back(0.724609); 
    m3_signif_T1tttt_1500_100.push_back(2.68756); m3_limit_T1tttt_1500_100.push_back(0.533203); m3_signif_T1tttt_1200_800.push_back(2.97085); m3_limit_T1tttt_1200_800.push_back(0.591797); 
    m3_signif_T1tttt_1500_100.push_back(2.89114); m3_limit_T1tttt_1500_100.push_back(0.458984); m3_signif_T1tttt_1200_800.push_back(3.20466); m3_limit_T1tttt_1200_800.push_back(0.505859); 
    m3_signif_T1tttt_1500_100.push_back(3.04804); m3_limit_T1tttt_1500_100.push_back(0.404297); m3_signif_T1tttt_1200_800.push_back(3.39295); m3_limit_T1tttt_1200_800.push_back(0.447266); 
    m3_signif_T1tttt_1500_100.push_back(3.17883); m3_limit_T1tttt_1500_100.push_back(0.366211); m3_signif_T1tttt_1200_800.push_back(3.5525); m3_limit_T1tttt_1200_800.push_back(0.404297); 
    m3_signif_T1tttt_1500_100.push_back(3.29148); m3_limit_T1tttt_1500_100.push_back(0.336914); m3_signif_T1tttt_1200_800.push_back(3.68931); m3_limit_T1tttt_1200_800.push_back(0.370117); 
    m3_signif_T1tttt_1500_100.push_back(3.38794); m3_limit_T1tttt_1500_100.push_back(0.313477); m3_signif_T1tttt_1200_800.push_back(3.80866); m3_limit_T1tttt_1200_800.push_back(0.344727); 
    m3_signif_T1tttt_1500_100.push_back(3.47317); m3_limit_T1tttt_1500_100.push_back(0.293945); m3_signif_T1tttt_1200_800.push_back(3.91435); m3_limit_T1tttt_1200_800.push_back(0.323242); 
    m3_signif_T1tttt_1500_100.push_back(3.55381); m3_limit_T1tttt_1500_100.push_back(0.27832); m3_signif_T1tttt_1200_800.push_back(4.0103); m3_limit_T1tttt_1200_800.push_back(0.305664); 
    m3_signif_T1tttt_1500_100.push_back(3.62492); m3_limit_T1tttt_1500_100.push_back(0.264648); m3_signif_T1tttt_1200_800.push_back(4.09615); m3_limit_T1tttt_1200_800.push_back(0.290039); 
    m3_signif_T1tttt_1500_100.push_back(3.69081); m3_limit_T1tttt_1500_100.push_back(0.25293); m3_signif_T1tttt_1200_800.push_back(4.17473); m3_limit_T1tttt_1200_800.push_back(0.276367); 
    m3_signif_T1tttt_1500_100.push_back(3.75036); m3_limit_T1tttt_1500_100.push_back(0.243164); m3_signif_T1tttt_1200_800.push_back(4.24601); m3_limit_T1tttt_1200_800.push_back(0.266602); 
    m3_signif_T1tttt_1500_100.push_back(3.80477); m3_limit_T1tttt_1500_100.push_back(0.233398); m3_signif_T1tttt_1200_800.push_back(4.31189); m3_limit_T1tttt_1200_800.push_back(0.255859); 
    m3_signif_T1tttt_1500_100.push_back(3.85662); m3_limit_T1tttt_1500_100.push_back(0.225586); m3_signif_T1tttt_1200_800.push_back(4.37296); m3_limit_T1tttt_1200_800.push_back(0.24707); 
    m3_signif_T1tttt_1500_100.push_back(3.90088); m3_limit_T1tttt_1500_100.push_back(0.217773); m3_signif_T1tttt_1200_800.push_back(4.42785); m3_limit_T1tttt_1200_800.push_back(0.239258); 
    m3_signif_T1tttt_1500_100.push_back(3.94335); m3_limit_T1tttt_1500_100.push_back(0.211914); m3_signif_T1tttt_1200_800.push_back(4.47913); m3_limit_T1tttt_1200_800.push_back(0.231445); 
    m3_signif_T1tttt_1500_100.push_back(3.98429); m3_limit_T1tttt_1500_100.push_back(0.206055); m3_signif_T1tttt_1200_800.push_back(4.52764); m3_limit_T1tttt_1200_800.push_back(0.225586); 
    m3_signif_T1tttt_1500_100.push_back(4.02254); m3_limit_T1tttt_1500_100.push_back(0.200195); m3_signif_T1tttt_1200_800.push_back(4.57303); m3_limit_T1tttt_1200_800.push_back(0.219727); 
    break;
  case kSystOnly:
    m1_signif_T1tttt_1500_100.push_back(1.84738); m1_limit_T1tttt_1500_100.push_back(1.74219); m1_signif_T1tttt_1200_800.push_back(1.32479); m1_limit_T1tttt_1200_800.push_back(3.85938);
    m1_signif_T1tttt_1500_100.push_back(2.39414); m1_limit_T1tttt_1500_100.push_back(0.957031); m1_signif_T1tttt_1200_800.push_back(1.75427); m1_limit_T1tttt_1200_800.push_back(2.16406);
    m1_signif_T1tttt_1500_100.push_back(2.80696); m1_limit_T1tttt_1500_100.push_back(0.677734); m1_signif_T1tttt_1200_800.push_back(2.07508); m1_limit_T1tttt_1200_800.push_back(1.54297);
    m1_signif_T1tttt_1500_100.push_back(3.13429); m1_limit_T1tttt_1500_100.push_back(0.529297); m1_signif_T1tttt_1200_800.push_back(2.34773); m1_limit_T1tttt_1200_800.push_back(1.21484);
    m1_signif_T1tttt_1500_100.push_back(3.40822); m1_limit_T1tttt_1500_100.push_back(0.4375); m1_signif_T1tttt_1200_800.push_back(2.58224); m1_limit_T1tttt_1200_800.push_back(1.00391);
    m1_signif_T1tttt_1500_100.push_back(3.63767); m1_limit_T1tttt_1500_100.push_back(0.374023); m1_signif_T1tttt_1200_800.push_back(2.77762); m1_limit_T1tttt_1200_800.push_back(0.863281);
    m1_signif_T1tttt_1500_100.push_back(3.83464); m1_limit_T1tttt_1500_100.push_back(0.329102); m1_signif_T1tttt_1200_800.push_back(2.945); m1_limit_T1tttt_1200_800.push_back(0.755859);
    m1_signif_T1tttt_1500_100.push_back(4.01318); m1_limit_T1tttt_1500_100.push_back(0.293945); m1_signif_T1tttt_1200_800.push_back(3.09663); m1_limit_T1tttt_1200_800.push_back(0.673828);
    m1_signif_T1tttt_1500_100.push_back(4.17233); m1_limit_T1tttt_1500_100.push_back(0.266602); m1_signif_T1tttt_1200_800.push_back(3.23161); m1_limit_T1tttt_1200_800.push_back(0.607422);
    m1_signif_T1tttt_1500_100.push_back(4.31938); m1_limit_T1tttt_1500_100.push_back(0.243164); m1_signif_T1tttt_1200_800.push_back(3.35666); m1_limit_T1tttt_1200_800.push_back(0.556641);
    m1_signif_T1tttt_1500_100.push_back(4.45669); m1_limit_T1tttt_1500_100.push_back(0.225586); m1_signif_T1tttt_1200_800.push_back(3.47394); m1_limit_T1tttt_1200_800.push_back(0.509766);
    m1_signif_T1tttt_1500_100.push_back(4.58463); m1_limit_T1tttt_1500_100.push_back(0.209961); m1_signif_T1tttt_1200_800.push_back(3.58373); m1_limit_T1tttt_1200_800.push_back(0.474609);
    m1_signif_T1tttt_1500_100.push_back(4.70157); m1_limit_T1tttt_1500_100.push_back(0.196289); m1_signif_T1tttt_1200_800.push_back(3.68416); m1_limit_T1tttt_1200_800.push_back(0.443359);
    m1_signif_T1tttt_1500_100.push_back(4.81274); m1_limit_T1tttt_1500_100.push_back(0.184082); m1_signif_T1tttt_1200_800.push_back(3.77997); m1_limit_T1tttt_1200_800.push_back(0.412109);
    m1_signif_T1tttt_1500_100.push_back(4.91415); m1_limit_T1tttt_1500_100.push_back(0.174316); m1_signif_T1tttt_1200_800.push_back(3.86777); m1_limit_T1tttt_1200_800.push_back(0.390625);
    m1_signif_T1tttt_1500_100.push_back(5.01038); m1_limit_T1tttt_1500_100.push_back(0.164551); m1_signif_T1tttt_1200_800.push_back(3.9516); m1_limit_T1tttt_1200_800.push_back(0.368164);
    m1_signif_T1tttt_1500_100.push_back(5.1033); m1_limit_T1tttt_1500_100.push_back(0.156738); m1_signif_T1tttt_1200_800.push_back(4.03244); m1_limit_T1tttt_1200_800.push_back(0.348633);
    m1_signif_T1tttt_1500_100.push_back(5.19002); m1_limit_T1tttt_1500_100.push_back(0.149902); m1_signif_T1tttt_1200_800.push_back(4.1085); m1_limit_T1tttt_1200_800.push_back(0.333008);
    m1_signif_T1tttt_1500_100.push_back(5.2638); m1_limit_T1tttt_1500_100.push_back(0.143066); m1_signif_T1tttt_1200_800.push_back(4.1738); m1_limit_T1tttt_1200_800.push_back(0.317383);
    m1_signif_T1tttt_1500_100.push_back(5.32548); m1_limit_T1tttt_1500_100.push_back(0.137207); m1_signif_T1tttt_1200_800.push_back(4.22879); m1_limit_T1tttt_1200_800.push_back(0.303711);
    m2_signif_T1tttt_1500_100.push_back(1.37228); m2_limit_T1tttt_1500_100.push_back(1.57031); m2_signif_T1tttt_1200_800.push_back(1.32448); m2_limit_T1tttt_1200_800.push_back(1.9375);
    m2_signif_T1tttt_1500_100.push_back(1.88988); m2_limit_T1tttt_1500_100.push_back(0.910156); m2_signif_T1tttt_1200_800.push_back(1.82879); m2_limit_T1tttt_1200_800.push_back(1.16797);
    m2_signif_T1tttt_1500_100.push_back(2.28067); m2_limit_T1tttt_1500_100.push_back(0.669922); m2_signif_T1tttt_1200_800.push_back(2.20501); m2_limit_T1tttt_1200_800.push_back(0.878906);
    m2_signif_T1tttt_1500_100.push_back(2.58098); m2_limit_T1tttt_1500_100.push_back(0.544922); m2_signif_T1tttt_1200_800.push_back(2.49584); m2_limit_T1tttt_1200_800.push_back(0.716797);
    m2_signif_T1tttt_1500_100.push_back(2.81089); m2_limit_T1tttt_1500_100.push_back(0.464844); m2_signif_T1tttt_1200_800.push_back(2.73071); m2_limit_T1tttt_1200_800.push_back(0.611328);
    m2_signif_T1tttt_1500_100.push_back(2.99448); m2_limit_T1tttt_1500_100.push_back(0.408203); m2_signif_T1tttt_1200_800.push_back(2.92836); m2_limit_T1tttt_1200_800.push_back(0.541016);
    m2_signif_T1tttt_1500_100.push_back(3.15079); m2_limit_T1tttt_1500_100.push_back(0.366211); m2_signif_T1tttt_1200_800.push_back(3.10113); m2_limit_T1tttt_1200_800.push_back(0.484375);
    m2_signif_T1tttt_1500_100.push_back(3.28804); m2_limit_T1tttt_1500_100.push_back(0.334961); m2_signif_T1tttt_1200_800.push_back(3.25487); m2_limit_T1tttt_1200_800.push_back(0.443359);
    m2_signif_T1tttt_1500_100.push_back(3.40779); m2_limit_T1tttt_1500_100.push_back(0.30957); m2_signif_T1tttt_1200_800.push_back(3.39294); m2_limit_T1tttt_1200_800.push_back(0.408203);
    m2_signif_T1tttt_1500_100.push_back(3.51528); m2_limit_T1tttt_1500_100.push_back(0.288086); m2_signif_T1tttt_1200_800.push_back(3.51901); m2_limit_T1tttt_1200_800.push_back(0.37793);
    m2_signif_T1tttt_1500_100.push_back(3.61764); m2_limit_T1tttt_1500_100.push_back(0.270508); m2_signif_T1tttt_1200_800.push_back(3.63677); m2_limit_T1tttt_1200_800.push_back(0.354492);
    m2_signif_T1tttt_1500_100.push_back(3.70888); m2_limit_T1tttt_1500_100.push_back(0.254883); m2_signif_T1tttt_1200_800.push_back(3.74437); m2_limit_T1tttt_1200_800.push_back(0.334961);
    m2_signif_T1tttt_1500_100.push_back(3.79375); m2_limit_T1tttt_1500_100.push_back(0.241211); m2_signif_T1tttt_1200_800.push_back(3.84442); m2_limit_T1tttt_1200_800.push_back(0.317383);
    m2_signif_T1tttt_1500_100.push_back(3.87144); m2_limit_T1tttt_1500_100.push_back(0.231445); m2_signif_T1tttt_1200_800.push_back(3.93751); m2_limit_T1tttt_1200_800.push_back(0.301758);
    m2_signif_T1tttt_1500_100.push_back(3.94293); m2_limit_T1tttt_1500_100.push_back(0.219727); m2_signif_T1tttt_1200_800.push_back(4.02467); m2_limit_T1tttt_1200_800.push_back(0.288086);
    m2_signif_T1tttt_1500_100.push_back(4.01115); m2_limit_T1tttt_1500_100.push_back(0.211914); m2_signif_T1tttt_1200_800.push_back(4.10682); m2_limit_T1tttt_1200_800.push_back(0.276367);
    m2_signif_T1tttt_1500_100.push_back(4.07119); m2_limit_T1tttt_1500_100.push_back(0.204102); m2_signif_T1tttt_1200_800.push_back(4.18362); m2_limit_T1tttt_1200_800.push_back(0.264648);
    m2_signif_T1tttt_1500_100.push_back(4.12849); m2_limit_T1tttt_1500_100.push_back(0.196289); m2_signif_T1tttt_1200_800.push_back(4.25591); m2_limit_T1tttt_1200_800.push_back(0.254883);
    m2_signif_T1tttt_1500_100.push_back(4.18418); m2_limit_T1tttt_1500_100.push_back(0.189941); m2_signif_T1tttt_1200_800.push_back(4.32552); m2_limit_T1tttt_1200_800.push_back(0.24707);
    m2_signif_T1tttt_1500_100.push_back(4.23664); m2_limit_T1tttt_1500_100.push_back(0.184082); m2_signif_T1tttt_1200_800.push_back(4.39189); m2_limit_T1tttt_1200_800.push_back(0.239258);
    m3_signif_T1tttt_1500_100.push_back(1.43625); m3_limit_T1tttt_1500_100.push_back(1.48047); m3_signif_T1tttt_1200_800.push_back(1.55122); m3_limit_T1tttt_1200_800.push_back(1.69531);
    m3_signif_T1tttt_1500_100.push_back(1.98114); m3_limit_T1tttt_1500_100.push_back(0.863281); m3_signif_T1tttt_1200_800.push_back(2.15234); m3_limit_T1tttt_1200_800.push_back(0.988281);
    m3_signif_T1tttt_1500_100.push_back(2.39264); m3_limit_T1tttt_1500_100.push_back(0.638672); m3_signif_T1tttt_1200_800.push_back(2.59946); m3_limit_T1tttt_1200_800.push_back(0.728516);
    m3_signif_T1tttt_1500_100.push_back(2.71151); m3_limit_T1tttt_1500_100.push_back(0.521484); m3_signif_T1tttt_1200_800.push_back(2.95267); m3_limit_T1tttt_1200_800.push_back(0.587891);
    m3_signif_T1tttt_1500_100.push_back(2.95919); m3_limit_T1tttt_1500_100.push_back(0.447266); m3_signif_T1tttt_1200_800.push_back(3.24439); m3_limit_T1tttt_1200_800.push_back(0.501953);
    m3_signif_T1tttt_1500_100.push_back(3.16025); m3_limit_T1tttt_1500_100.push_back(0.392578); m3_signif_T1tttt_1200_800.push_back(3.49507); m3_limit_T1tttt_1200_800.push_back(0.439453);
    m3_signif_T1tttt_1500_100.push_back(3.33323); m3_limit_T1tttt_1500_100.push_back(0.352539); m3_signif_T1tttt_1200_800.push_back(3.71751); m3_limit_T1tttt_1200_800.push_back(0.394531);
    m3_signif_T1tttt_1500_100.push_back(3.48648); m3_limit_T1tttt_1500_100.push_back(0.321289); m3_signif_T1tttt_1200_800.push_back(3.91775); m3_limit_T1tttt_1200_800.push_back(0.360352);
    m3_signif_T1tttt_1500_100.push_back(3.62174); m3_limit_T1tttt_1500_100.push_back(0.297852); m3_signif_T1tttt_1200_800.push_back(4.1001); m3_limit_T1tttt_1200_800.push_back(0.331055);
    m3_signif_T1tttt_1500_100.push_back(3.74431); m3_limit_T1tttt_1500_100.push_back(0.27832); m3_signif_T1tttt_1200_800.push_back(4.26816); m3_limit_T1tttt_1200_800.push_back(0.30957);
    m3_signif_T1tttt_1500_100.push_back(3.86096); m3_limit_T1tttt_1500_100.push_back(0.260742); m3_signif_T1tttt_1200_800.push_back(4.42532); m3_limit_T1tttt_1200_800.push_back(0.290039);
    m3_signif_T1tttt_1500_100.push_back(3.96608); m3_limit_T1tttt_1500_100.push_back(0.24707); m3_signif_T1tttt_1200_800.push_back(4.57093); m3_limit_T1tttt_1200_800.push_back(0.272461);
    m3_signif_T1tttt_1500_100.push_back(4.06435); m3_limit_T1tttt_1500_100.push_back(0.233398); m3_signif_T1tttt_1200_800.push_back(4.7075); m3_limit_T1tttt_1200_800.push_back(0.258789);
    m3_signif_T1tttt_1500_100.push_back(4.15497); m3_limit_T1tttt_1500_100.push_back(0.223633); m3_signif_T1tttt_1200_800.push_back(4.83558); m3_limit_T1tttt_1200_800.push_back(0.24707);
    m3_signif_T1tttt_1500_100.push_back(4.23934); m3_limit_T1tttt_1500_100.push_back(0.213867); m3_signif_T1tttt_1200_800.push_back(4.95707); m3_limit_T1tttt_1200_800.push_back(0.235352);
    m3_signif_T1tttt_1500_100.push_back(4.32016); m3_limit_T1tttt_1500_100.push_back(0.206055); m3_signif_T1tttt_1200_800.push_back(5.07237); m3_limit_T1tttt_1200_800.push_back(0.225586);
    m3_signif_T1tttt_1500_100.push_back(4.39284); m3_limit_T1tttt_1500_100.push_back(0.198242); m3_signif_T1tttt_1200_800.push_back(5.18135); m3_limit_T1tttt_1200_800.push_back(0.217773);
    m3_signif_T1tttt_1500_100.push_back(4.46253); m3_limit_T1tttt_1500_100.push_back(0.190918); m3_signif_T1tttt_1200_800.push_back(5.28508); m3_limit_T1tttt_1200_800.push_back(0.209961);
    m3_signif_T1tttt_1500_100.push_back(4.53002); m3_limit_T1tttt_1500_100.push_back(0.184082); m3_signif_T1tttt_1200_800.push_back(5.3848); m3_limit_T1tttt_1200_800.push_back(0.202148);
    m3_signif_T1tttt_1500_100.push_back(4.59412); m3_limit_T1tttt_1500_100.push_back(0.178711); m3_signif_T1tttt_1200_800.push_back(5.48038); m3_limit_T1tttt_1200_800.push_back(0.196289);
    break;
  default: break;
  }

  switch(method){
  case 1:
    ncy = m1_signif_T1tttt_1500_100;
    cy = m1_signif_T1tttt_1200_800;
    ncz = m1_limit_T1tttt_1500_100;
    cz = m1_limit_T1tttt_1200_800;
    break;
  case 2:
    ncy = m2_signif_T1tttt_1500_100;
    cy = m2_signif_T1tttt_1200_800;
    ncz = m2_limit_T1tttt_1500_100;
    cz = m2_limit_T1tttt_1200_800;
    break;
  case 3:
    ncy = m3_signif_T1tttt_1500_100;
    cy = m3_signif_T1tttt_1200_800;
    ncz = m3_limit_T1tttt_1500_100;
    cz = m3_limit_T1tttt_1200_800;
    break;
  default:
    break;
  }

  double big_num = 10.;
  x.clear();
  for(size_t i = 0; i < ncy.size(); ++i){
    x.push_back(i+1.);
    if(ncz.at(i)<=0.){
      ncz.at(i) = big_num;
    }
    ncy.at(i) *= 0.25;
    if(cz.at(i) <= 0.){
      cz.at(i) = big_num;
    }
    cy.at(i) *= 0.25;
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
  TH1D h("h", ";Luminosity [fb^{-1}];Expected Limit/X-Section", 1, 0., 15.);
  h.SetMaximum(2.);
  TAxis &yaxis = *h.GetYaxis();
  yaxis.SetLabelColor(kBlue);
  yaxis.SetTitleColor(kBlue);
  yaxis.SetTitleOffset(.8);
  TCanvas c;
  c.SetTicks(1,0);
  TGaxis *raxis = new TGaxis(15., 0.,
                             15., 2.,
                             0, 8., 510, "+L");
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
  case kAll: oss << "_all_uncerts"; break;
  case kStatOnly: oss << "_stat_only"; break;
  case kSystOnly: oss << "_syst_only"; break;
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
      case 0: uncert_set = kAll; break;
      case 1: uncert_set = kStatOnly; break;
      case 2: uncert_set = kSystOnly; break;
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
