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
m1_signif_T1tttt_1500_100.push_back(2.2289); m1_limit_T1tttt_1500_100.push_back(1.71094); m1_signif_T1tttt_1200_800.push_back(1.54454); m1_limit_T1tttt_1200_800.push_back(3.78125); 
m1_signif_T1tttt_1500_100.push_back(2.67221); m1_limit_T1tttt_1500_100.push_back(0.941406); m1_signif_T1tttt_1200_800.push_back(1.91044); m1_limit_T1tttt_1200_800.push_back(2.11719); 
m1_signif_T1tttt_1500_100.push_back(2.91589); m1_limit_T1tttt_1500_100.push_back(0.671875); m1_signif_T1tttt_1200_800.push_back(2.11732); m1_limit_T1tttt_1200_800.push_back(1.51953); 
m1_signif_T1tttt_1500_100.push_back(3.08392); m1_limit_T1tttt_1500_100.push_back(0.533203); m1_signif_T1tttt_1200_800.push_back(2.25872); m1_limit_T1tttt_1200_800.push_back(1.20703); 
m1_signif_T1tttt_1500_100.push_back(3.21246); m1_limit_T1tttt_1500_100.push_back(0.447266); m1_signif_T1tttt_1200_800.push_back(2.36561); m1_limit_T1tttt_1200_800.push_back(1.01172); 
m1_signif_T1tttt_1500_100.push_back(3.31662); m1_limit_T1tttt_1500_100.push_back(0.390625); m1_signif_T1tttt_1200_800.push_back(2.4515); m1_limit_T1tttt_1200_800.push_back(0.878906); 
m1_signif_T1tttt_1500_100.push_back(3.40412); m1_limit_T1tttt_1500_100.push_back(0.348633); m1_signif_T1tttt_1200_800.push_back(2.5233); m1_limit_T1tttt_1200_800.push_back(0.785156); 
m1_signif_T1tttt_1500_100.push_back(3.47949); m1_limit_T1tttt_1500_100.push_back(0.31543); m1_signif_T1tttt_1200_800.push_back(2.58499); m1_limit_T1tttt_1200_800.push_back(0.712891); 
m1_signif_T1tttt_1500_100.push_back(3.5456); m1_limit_T1tttt_1500_100.push_back(0.291016); m1_signif_T1tttt_1200_800.push_back(2.63904); m1_limit_T1tttt_1200_800.push_back(0.654297); 
m1_signif_T1tttt_1500_100.push_back(3.6044); m1_limit_T1tttt_1500_100.push_back(0.270508); m1_signif_T1tttt_1200_800.push_back(2.68712); m1_limit_T1tttt_1200_800.push_back(0.607422); 
m1_signif_T1tttt_1500_100.push_back(3.65727); m1_limit_T1tttt_1500_100.push_back(0.254883); m1_signif_T1tttt_1200_800.push_back(2.73039); m1_limit_T1tttt_1200_800.push_back(0.568359); 
m1_signif_T1tttt_1500_100.push_back(3.70525); m1_limit_T1tttt_1500_100.push_back(0.241211); m1_signif_T1tttt_1200_800.push_back(2.76969); m1_limit_T1tttt_1200_800.push_back(0.533203); 
m1_signif_T1tttt_1500_100.push_back(3.74912); m1_limit_T1tttt_1500_100.push_back(0.227539); m1_signif_T1tttt_1200_800.push_back(2.80567); m1_limit_T1tttt_1200_800.push_back(0.505859); 
m1_signif_T1tttt_1500_100.push_back(3.78946); m1_limit_T1tttt_1500_100.push_back(0.217773); m1_signif_T1tttt_1200_800.push_back(2.83881); m1_limit_T1tttt_1200_800.push_back(0.482422); 
m1_signif_T1tttt_1500_100.push_back(3.82678); m1_limit_T1tttt_1500_100.push_back(0.209961); m1_signif_T1tttt_1200_800.push_back(2.8695); m1_limit_T1tttt_1200_800.push_back(0.458984); 
m1_signif_T1tttt_1500_100.push_back(3.86146); m1_limit_T1tttt_1500_100.push_back(0.202148); m1_signif_T1tttt_1200_800.push_back(2.89807); m1_limit_T1tttt_1200_800.push_back(0.443359); 
m1_signif_T1tttt_1500_100.push_back(3.89381); m1_limit_T1tttt_1500_100.push_back(0.193848); m1_signif_T1tttt_1200_800.push_back(2.92475); m1_limit_T1tttt_1200_800.push_back(0.423828); 
m1_signif_T1tttt_1500_100.push_back(3.92408); m1_limit_T1tttt_1500_100.push_back(0.187988); m1_signif_T1tttt_1200_800.push_back(2.94976); m1_limit_T1tttt_1200_800.push_back(0.412109); 
m1_signif_T1tttt_1500_100.push_back(3.95253); m1_limit_T1tttt_1500_100.push_back(0.182129); m1_signif_T1tttt_1200_800.push_back(2.97329); m1_limit_T1tttt_1200_800.push_back(0.396484); 
m1_signif_T1tttt_1500_100.push_back(3.97933); m1_limit_T1tttt_1500_100.push_back(0.177246); m1_signif_T1tttt_1200_800.push_back(2.99548); m1_limit_T1tttt_1200_800.push_back(0.385742); 
m2_signif_T1tttt_1500_100.push_back(1.67715); m2_limit_T1tttt_1500_100.push_back(1.57031); m2_signif_T1tttt_1200_800.push_back(1.45094); m2_limit_T1tttt_1200_800.push_back(1.92969); 
m2_signif_T1tttt_1500_100.push_back(1.96074); m2_limit_T1tttt_1500_100.push_back(0.933594); m2_signif_T1tttt_1200_800.push_back(1.73253); m2_limit_T1tttt_1200_800.push_back(1.19141); 
m2_signif_T1tttt_1500_100.push_back(2.11749); m2_limit_T1tttt_1500_100.push_back(0.701172); m2_signif_T1tttt_1200_800.push_back(1.88947); m2_limit_T1tttt_1200_800.push_back(0.910156); 
m2_signif_T1tttt_1500_100.push_back(2.22645); m2_limit_T1tttt_1500_100.push_back(0.580078); m2_signif_T1tttt_1200_800.push_back(1.99821); m2_limit_T1tttt_1200_800.push_back(0.751953); 
m2_signif_T1tttt_1500_100.push_back(2.31004); m2_limit_T1tttt_1500_100.push_back(0.501953); m2_signif_T1tttt_1200_800.push_back(2.08147); m2_limit_T1tttt_1200_800.push_back(0.654297); 
m2_signif_T1tttt_1500_100.push_back(2.37776); m2_limit_T1tttt_1500_100.push_back(0.451172); m2_signif_T1tttt_1200_800.push_back(2.14893); m2_limit_T1tttt_1200_800.push_back(0.583984); 
m2_signif_T1tttt_1500_100.push_back(2.43452); m2_limit_T1tttt_1500_100.push_back(0.412109); m2_signif_T1tttt_1200_800.push_back(2.20558); m2_limit_T1tttt_1200_800.push_back(0.533203); 
m2_signif_T1tttt_1500_100.push_back(2.4832); m2_limit_T1tttt_1500_100.push_back(0.379883); m2_signif_T1tttt_1200_800.push_back(2.25434); m2_limit_T1tttt_1200_800.push_back(0.490234); 
m2_signif_T1tttt_1500_100.push_back(2.52569); m2_limit_T1tttt_1500_100.push_back(0.356445); m2_signif_T1tttt_1200_800.push_back(2.29707); m2_limit_T1tttt_1200_800.push_back(0.458984); 
m2_signif_T1tttt_1500_100.push_back(2.56326); m2_limit_T1tttt_1500_100.push_back(0.335938); m2_signif_T1tttt_1200_800.push_back(2.33505); m2_limit_T1tttt_1200_800.push_back(0.431641); 
m2_signif_T1tttt_1500_100.push_back(2.59683); m2_limit_T1tttt_1500_100.push_back(0.319336); m2_signif_T1tttt_1200_800.push_back(2.36915); m2_limit_T1tttt_1200_800.push_back(0.408203); 
m2_signif_T1tttt_1500_100.push_back(2.62707); m2_limit_T1tttt_1500_100.push_back(0.304688); m2_signif_T1tttt_1200_800.push_back(2.40005); m2_limit_T1tttt_1200_800.push_back(0.389648); 
m2_signif_T1tttt_1500_100.push_back(2.65452); m2_limit_T1tttt_1500_100.push_back(0.291992); m2_signif_T1tttt_1200_800.push_back(2.42826); m2_limit_T1tttt_1200_800.push_back(0.37207); 
m2_signif_T1tttt_1500_100.push_back(2.67957); m2_limit_T1tttt_1500_100.push_back(0.280273); m2_signif_T1tttt_1200_800.push_back(2.45416); m2_limit_T1tttt_1200_800.push_back(0.357422); 
m2_signif_T1tttt_1500_100.push_back(2.70256); m2_limit_T1tttt_1500_100.push_back(0.270508); m2_signif_T1tttt_1200_800.push_back(2.47805); m2_limit_T1tttt_1200_800.push_back(0.344727); 
m2_signif_T1tttt_1500_100.push_back(2.72375); m2_limit_T1tttt_1500_100.push_back(0.262695); m2_signif_T1tttt_1200_800.push_back(2.50023); m2_limit_T1tttt_1200_800.push_back(0.333008); 
m2_signif_T1tttt_1500_100.push_back(2.74336); m2_limit_T1tttt_1500_100.push_back(0.254883); m2_signif_T1tttt_1200_800.push_back(2.52086); m2_limit_T1tttt_1200_800.push_back(0.322266); 
m2_signif_T1tttt_1500_100.push_back(2.76155); m2_limit_T1tttt_1500_100.push_back(0.24707); m2_signif_T1tttt_1200_800.push_back(2.54012); m2_limit_T1tttt_1200_800.push_back(0.313477); 
m2_signif_T1tttt_1500_100.push_back(2.77852); m2_limit_T1tttt_1500_100.push_back(0.241211); m2_signif_T1tttt_1200_800.push_back(2.55819); m2_limit_T1tttt_1200_800.push_back(0.303711); 
m2_signif_T1tttt_1500_100.push_back(2.79437); m2_limit_T1tttt_1500_100.push_back(0.235352); m2_signif_T1tttt_1200_800.push_back(2.57517); m2_limit_T1tttt_1200_800.push_back(0.295898); 
m3_signif_T1tttt_1500_100.push_back(1.73853); m3_limit_T1tttt_1500_100.push_back(1.48828); m3_signif_T1tttt_1200_800.push_back(1.67391); m3_limit_T1tttt_1200_800.push_back(1.69531); 
m3_signif_T1tttt_1500_100.push_back(2.05613); m3_limit_T1tttt_1500_100.push_back(0.886719); m3_signif_T1tttt_1200_800.push_back(2.05615); m3_limit_T1tttt_1200_800.push_back(1.01172); 
m3_signif_T1tttt_1500_100.push_back(2.24152); m3_limit_T1tttt_1500_100.push_back(0.669922); m3_signif_T1tttt_1200_800.push_back(2.29024); m3_limit_T1tttt_1200_800.push_back(0.759766); 
m3_signif_T1tttt_1500_100.push_back(2.37516); m3_limit_T1tttt_1500_100.push_back(0.556641); m3_signif_T1tttt_1200_800.push_back(2.46108); m3_limit_T1tttt_1200_800.push_back(0.623047); 
m3_signif_T1tttt_1500_100.push_back(2.48032); m3_limit_T1tttt_1500_100.push_back(0.484375); m3_signif_T1tttt_1200_800.push_back(2.59572); m3_limit_T1tttt_1200_800.push_back(0.541016); 
m3_signif_T1tttt_1500_100.push_back(2.5671); m3_limit_T1tttt_1500_100.push_back(0.435547); m3_signif_T1tttt_1200_800.push_back(2.70649); m3_limit_T1tttt_1200_800.push_back(0.478516); 
m3_signif_T1tttt_1500_100.push_back(2.64085); m3_limit_T1tttt_1500_100.push_back(0.396484); m3_signif_T1tttt_1200_800.push_back(2.80021); m3_limit_T1tttt_1200_800.push_back(0.435547); 
m3_signif_T1tttt_1500_100.push_back(2.70481); m3_limit_T1tttt_1500_100.push_back(0.368164); m3_signif_T1tttt_1200_800.push_back(2.88108); m3_limit_T1tttt_1200_800.push_back(0.404297); 
m3_signif_T1tttt_1500_100.push_back(2.76111); m3_limit_T1tttt_1500_100.push_back(0.344727); m3_signif_T1tttt_1200_800.push_back(2.95189); m3_limit_T1tttt_1200_800.push_back(0.375977); 
m3_signif_T1tttt_1500_100.push_back(2.81124); m3_limit_T1tttt_1500_100.push_back(0.325195); m3_signif_T1tttt_1200_800.push_back(3.01465); m3_limit_T1tttt_1200_800.push_back(0.354492); 
m3_signif_T1tttt_1500_100.push_back(2.85628); m3_limit_T1tttt_1500_100.push_back(0.30957); m3_signif_T1tttt_1200_800.push_back(3.07079); m3_limit_T1tttt_1200_800.push_back(0.334961); 
m3_signif_T1tttt_1500_100.push_back(2.89704); m3_limit_T1tttt_1500_100.push_back(0.295898); m3_signif_T1tttt_1200_800.push_back(3.1214); m3_limit_T1tttt_1200_800.push_back(0.319336); 
m3_signif_T1tttt_1500_100.push_back(2.93418); m3_limit_T1tttt_1500_100.push_back(0.28418); m3_signif_T1tttt_1200_800.push_back(3.16734); m3_limit_T1tttt_1200_800.push_back(0.305664); 
m3_signif_T1tttt_1500_100.push_back(2.96819); m3_limit_T1tttt_1500_100.push_back(0.273438); m3_signif_T1tttt_1200_800.push_back(3.20927); m3_limit_T1tttt_1200_800.push_back(0.293945); 
m3_signif_T1tttt_1500_100.push_back(2.99949); m3_limit_T1tttt_1500_100.push_back(0.264648); m3_signif_T1tttt_1200_800.push_back(3.24774); m3_limit_T1tttt_1200_800.push_back(0.28418); 
m3_signif_T1tttt_1500_100.push_back(3.02841); m3_limit_T1tttt_1500_100.push_back(0.256836); m3_signif_T1tttt_1200_800.push_back(3.2832); m3_limit_T1tttt_1200_800.push_back(0.274414); 
m3_signif_T1tttt_1500_100.push_back(3.05523); m3_limit_T1tttt_1500_100.push_back(0.249023); m3_signif_T1tttt_1200_800.push_back(3.31601); m3_limit_T1tttt_1200_800.push_back(0.266602); 
m3_signif_T1tttt_1500_100.push_back(3.08017); m3_limit_T1tttt_1500_100.push_back(0.241211); m3_signif_T1tttt_1200_800.push_back(3.34646); m3_limit_T1tttt_1200_800.push_back(0.258789); 
m3_signif_T1tttt_1500_100.push_back(3.10344); m3_limit_T1tttt_1500_100.push_back(0.235352); m3_signif_T1tttt_1200_800.push_back(3.37484); m3_limit_T1tttt_1200_800.push_back(0.25293); 
m3_signif_T1tttt_1500_100.push_back(3.12523); m3_limit_T1tttt_1500_100.push_back(0.229492); m3_signif_T1tttt_1200_800.push_back(3.40137); m3_limit_T1tttt_1200_800.push_back(0.24707); 
    break;
  case kStatOnly:
m1_signif_T1tttt_1500_100.push_back(2.2387); m1_limit_T1tttt_1500_100.push_back(1.71094); m1_signif_T1tttt_1200_800.push_back(1.54565); m1_limit_T1tttt_1200_800.push_back(3.76562); 
m1_signif_T1tttt_1500_100.push_back(2.69725); m1_limit_T1tttt_1500_100.push_back(0.941406); m1_signif_T1tttt_1200_800.push_back(1.91319); m1_limit_T1tttt_1200_800.push_back(2.11719); 
m1_signif_T1tttt_1500_100.push_back(2.95759); m1_limit_T1tttt_1500_100.push_back(0.669922); m1_signif_T1tttt_1200_800.push_back(2.12207); m1_limit_T1tttt_1200_800.push_back(1.51562); 
m1_signif_T1tttt_1500_100.push_back(3.14225); m1_limit_T1tttt_1500_100.push_back(0.529297); m1_signif_T1tttt_1200_800.push_back(2.26574); m1_limit_T1tttt_1200_800.push_back(1.20703); 
m1_signif_T1tttt_1500_100.push_back(3.28682); m1_limit_T1tttt_1500_100.push_back(0.443359); m1_signif_T1tttt_1200_800.push_back(2.37509); m1_limit_T1tttt_1200_800.push_back(1.01172); 
m1_signif_T1tttt_1500_100.push_back(3.4062); m1_limit_T1tttt_1500_100.push_back(0.385742); m1_signif_T1tttt_1200_800.push_back(2.46355); m1_limit_T1tttt_1200_800.push_back(0.878906); 
m1_signif_T1tttt_1500_100.push_back(3.5081); m1_limit_T1tttt_1500_100.push_back(0.344727); m1_signif_T1tttt_1200_800.push_back(2.53799); m1_limit_T1tttt_1200_800.push_back(0.785156); 
m1_signif_T1tttt_1500_100.push_back(3.59702); m1_limit_T1tttt_1500_100.push_back(0.313477); m1_signif_T1tttt_1200_800.push_back(2.60234); m1_limit_T1tttt_1200_800.push_back(0.708984); 
m1_signif_T1tttt_1500_100.push_back(3.67591); m1_limit_T1tttt_1500_100.push_back(0.288086); m1_signif_T1tttt_1200_800.push_back(2.65907); m1_limit_T1tttt_1200_800.push_back(0.650391); 
m1_signif_T1tttt_1500_100.push_back(3.74674); m1_limit_T1tttt_1500_100.push_back(0.268555); m1_signif_T1tttt_1200_800.push_back(2.70982); m1_limit_T1tttt_1200_800.push_back(0.603516); 
m1_signif_T1tttt_1500_100.push_back(3.81096); m1_limit_T1tttt_1500_100.push_back(0.250977); m1_signif_T1tttt_1200_800.push_back(2.75572); m1_limit_T1tttt_1200_800.push_back(0.564453); 
m1_signif_T1tttt_1500_100.push_back(3.86968); m1_limit_T1tttt_1500_100.push_back(0.237305); m1_signif_T1tttt_1200_800.push_back(2.79761); m1_limit_T1tttt_1200_800.push_back(0.53125); 
m1_signif_T1tttt_1500_100.push_back(3.92369); m1_limit_T1tttt_1500_100.push_back(0.225586); m1_signif_T1tttt_1200_800.push_back(2.83615); m1_limit_T1tttt_1200_800.push_back(0.501953); 
m1_signif_T1tttt_1500_100.push_back(3.97364); m1_limit_T1tttt_1500_100.push_back(0.21582); m1_signif_T1tttt_1200_800.push_back(2.87178); m1_limit_T1tttt_1200_800.push_back(0.478516); 
m1_signif_T1tttt_1500_100.push_back(4.02009); m1_limit_T1tttt_1500_100.push_back(0.206055); m1_signif_T1tttt_1200_800.push_back(2.90492); m1_limit_T1tttt_1200_800.push_back(0.458984); 
m1_signif_T1tttt_1500_100.push_back(4.06344); m1_limit_T1tttt_1500_100.push_back(0.198242); m1_signif_T1tttt_1200_800.push_back(2.93588); m1_limit_T1tttt_1200_800.push_back(0.439453); 
m1_signif_T1tttt_1500_100.push_back(4.10403); m1_limit_T1tttt_1500_100.push_back(0.191406); m1_signif_T1tttt_1200_800.push_back(2.9649); m1_limit_T1tttt_1200_800.push_back(0.423828); 
m1_signif_T1tttt_1500_100.push_back(4.14215); m1_limit_T1tttt_1500_100.push_back(0.185059); m1_signif_T1tttt_1200_800.push_back(2.99218); m1_limit_T1tttt_1200_800.push_back(0.408203); 
m1_signif_T1tttt_1500_100.push_back(4.1781); m1_limit_T1tttt_1500_100.push_back(0.179199); m1_signif_T1tttt_1200_800.push_back(3.01793); m1_limit_T1tttt_1200_800.push_back(0.396484); 
m1_signif_T1tttt_1500_100.push_back(4.21206); m1_limit_T1tttt_1500_100.push_back(0.174316); m1_signif_T1tttt_1200_800.push_back(3.04229); m1_limit_T1tttt_1200_800.push_back(0.383789); 
m2_signif_T1tttt_1500_100.push_back(1.80007); m2_limit_T1tttt_1500_100.push_back(1.55859); m2_signif_T1tttt_1200_800.push_back(1.54269); m2_limit_T1tttt_1200_800.push_back(1.91406); 
m2_signif_T1tttt_1500_100.push_back(2.20397); m2_limit_T1tttt_1500_100.push_back(0.917969); m2_signif_T1tttt_1200_800.push_back(1.92866); m2_limit_T1tttt_1200_800.push_back(1.17578); 
m2_signif_T1tttt_1500_100.push_back(2.45456); m2_limit_T1tttt_1500_100.push_back(0.689453); m2_signif_T1tttt_1200_800.push_back(2.17366); m2_limit_T1tttt_1200_800.push_back(0.890625); 
m2_signif_T1tttt_1500_100.push_back(2.64092); m2_limit_T1tttt_1500_100.push_back(0.564453); m2_signif_T1tttt_1200_800.push_back(2.35729); m2_limit_T1tttt_1200_800.push_back(0.736328); 
m2_signif_T1tttt_1500_100.push_back(2.79092); m2_limit_T1tttt_1500_100.push_back(0.490234); m2_signif_T1tttt_1200_800.push_back(2.50577); m2_limit_T1tttt_1200_800.push_back(0.634766); 
m2_signif_T1tttt_1500_100.push_back(2.91709); m2_limit_T1tttt_1500_100.push_back(0.435547); m2_signif_T1tttt_1200_800.push_back(2.63115); m2_limit_T1tttt_1200_800.push_back(0.564453); 
m2_signif_T1tttt_1500_100.push_back(3.02618); m2_limit_T1tttt_1500_100.push_back(0.396484); m2_signif_T1tttt_1200_800.push_back(2.73998); m2_limit_T1tttt_1200_800.push_back(0.513672); 
m2_signif_T1tttt_1500_100.push_back(3.12233); m2_limit_T1tttt_1500_100.push_back(0.364258); m2_signif_T1tttt_1200_800.push_back(2.83627); m2_limit_T1tttt_1200_800.push_back(0.470703); 
m2_signif_T1tttt_1500_100.push_back(3.20825); m2_limit_T1tttt_1500_100.push_back(0.338867); m2_signif_T1tttt_1200_800.push_back(2.92267); m2_limit_T1tttt_1200_800.push_back(0.4375); 
m2_signif_T1tttt_1500_100.push_back(3.28587); m2_limit_T1tttt_1500_100.push_back(0.319336); m2_signif_T1tttt_1200_800.push_back(3.00109); m2_limit_T1tttt_1200_800.push_back(0.410156); 
m2_signif_T1tttt_1500_100.push_back(3.3566); m2_limit_T1tttt_1500_100.push_back(0.301758); m2_signif_T1tttt_1200_800.push_back(3.07283); m2_limit_T1tttt_1200_800.push_back(0.387695); 
m2_signif_T1tttt_1500_100.push_back(3.42148); m2_limit_T1tttt_1500_100.push_back(0.286133); m2_signif_T1tttt_1200_800.push_back(3.13894); m2_limit_T1tttt_1200_800.push_back(0.368164); 
m2_signif_T1tttt_1500_100.push_back(3.48138); m2_limit_T1tttt_1500_100.push_back(0.274414); m2_signif_T1tttt_1200_800.push_back(3.20024); m2_limit_T1tttt_1200_800.push_back(0.350586); 
m2_signif_T1tttt_1500_100.push_back(3.53691); m2_limit_T1tttt_1500_100.push_back(0.262695); m2_signif_T1tttt_1200_800.push_back(3.25733); m2_limit_T1tttt_1200_800.push_back(0.334961); 
m2_signif_T1tttt_1500_100.push_back(3.58865); m2_limit_T1tttt_1500_100.push_back(0.25293); m2_signif_T1tttt_1200_800.push_back(3.31075); m2_limit_T1tttt_1200_800.push_back(0.323242); 
m2_signif_T1tttt_1500_100.push_back(3.6368); m2_limit_T1tttt_1500_100.push_back(0.245117); m2_signif_T1tttt_1200_800.push_back(3.36092); m2_limit_T1tttt_1200_800.push_back(0.311523); 
m2_signif_T1tttt_1500_100.push_back(3.68175); m2_limit_T1tttt_1500_100.push_back(0.237305); m2_signif_T1tttt_1200_800.push_back(3.40818); m2_limit_T1tttt_1200_800.push_back(0.299805); 
m2_signif_T1tttt_1500_100.push_back(3.72382); m2_limit_T1tttt_1500_100.push_back(0.229492); m2_signif_T1tttt_1200_800.push_back(3.45281); m2_limit_T1tttt_1200_800.push_back(0.291992); 
m2_signif_T1tttt_1500_100.push_back(3.76335); m2_limit_T1tttt_1500_100.push_back(0.223633); m2_signif_T1tttt_1200_800.push_back(3.49511); m2_limit_T1tttt_1200_800.push_back(0.282227); 
m2_signif_T1tttt_1500_100.push_back(3.80058); m2_limit_T1tttt_1500_100.push_back(0.217773); m2_signif_T1tttt_1200_800.push_back(3.53528); m2_limit_T1tttt_1200_800.push_back(0.274414); 
m3_signif_T1tttt_1500_100.push_back(1.85776); m3_limit_T1tttt_1500_100.push_back(1.48047); m3_signif_T1tttt_1200_800.push_back(1.75452); m3_limit_T1tttt_1200_800.push_back(1.69531); 
m3_signif_T1tttt_1500_100.push_back(2.28896); m3_limit_T1tttt_1500_100.push_back(0.878906); m3_signif_T1tttt_1200_800.push_back(2.22339); m3_limit_T1tttt_1200_800.push_back(1.00391); 
m3_signif_T1tttt_1500_100.push_back(2.56143); m3_limit_T1tttt_1500_100.push_back(0.658203); m3_signif_T1tttt_1200_800.push_back(2.52832); m3_limit_T1tttt_1200_800.push_back(0.748047); 
m3_signif_T1tttt_1500_100.push_back(2.7661); m3_limit_T1tttt_1500_100.push_back(0.544922); m3_signif_T1tttt_1200_800.push_back(2.75847); m3_limit_T1tttt_1200_800.push_back(0.611328); 
m3_signif_T1tttt_1500_100.push_back(2.93179); m3_limit_T1tttt_1500_100.push_back(0.470703); m3_signif_T1tttt_1200_800.push_back(2.94438); m3_limit_T1tttt_1200_800.push_back(0.525391); 
m3_signif_T1tttt_1500_100.push_back(3.07161); m3_limit_T1tttt_1500_100.push_back(0.419922); m3_signif_T1tttt_1200_800.push_back(3.10054); m3_limit_T1tttt_1200_800.push_back(0.466797); 
m3_signif_T1tttt_1500_100.push_back(3.19271); m3_limit_T1tttt_1500_100.push_back(0.381836); m3_signif_T1tttt_1200_800.push_back(3.23514); m3_limit_T1tttt_1200_800.push_back(0.423828); 
m3_signif_T1tttt_1500_100.push_back(3.29952); m3_limit_T1tttt_1500_100.push_back(0.352539); m3_signif_T1tttt_1200_800.push_back(3.35328); m3_limit_T1tttt_1200_800.push_back(0.390625); 
m3_signif_T1tttt_1500_100.push_back(3.39499); m3_limit_T1tttt_1500_100.push_back(0.329102); m3_signif_T1tttt_1200_800.push_back(3.45843); m3_limit_T1tttt_1200_800.push_back(0.362305); 
m3_signif_T1tttt_1500_100.push_back(3.48123); m3_limit_T1tttt_1500_100.push_back(0.30957); m3_signif_T1tttt_1200_800.push_back(3.55304); m3_limit_T1tttt_1200_800.push_back(0.34082); 
m3_signif_T1tttt_1500_100.push_back(3.55976); m3_limit_T1tttt_1500_100.push_back(0.293945); m3_signif_T1tttt_1200_800.push_back(3.6389); m3_limit_T1tttt_1200_800.push_back(0.321289); 
m3_signif_T1tttt_1500_100.push_back(3.63175); m3_limit_T1tttt_1500_100.push_back(0.27832); m3_signif_T1tttt_1200_800.push_back(3.7174); m3_limit_T1tttt_1200_800.push_back(0.305664); 
m3_signif_T1tttt_1500_100.push_back(3.69815); m3_limit_T1tttt_1500_100.push_back(0.266602); m3_signif_T1tttt_1200_800.push_back(3.78962); m3_limit_T1tttt_1200_800.push_back(0.291992); 
m3_signif_T1tttt_1500_100.push_back(3.75965); m3_limit_T1tttt_1500_100.push_back(0.256836); m3_signif_T1tttt_1200_800.push_back(3.85639); m3_limit_T1tttt_1200_800.push_back(0.280273); 
m3_signif_T1tttt_1500_100.push_back(3.81691); m3_limit_T1tttt_1500_100.push_back(0.24707); m3_signif_T1tttt_1200_800.push_back(3.91843); m3_limit_T1tttt_1200_800.push_back(0.270508); 
m3_signif_T1tttt_1500_100.push_back(3.87017); m3_limit_T1tttt_1500_100.push_back(0.239258); m3_signif_T1tttt_1200_800.push_back(3.97632); m3_limit_T1tttt_1200_800.push_back(0.260742); 
m3_signif_T1tttt_1500_100.push_back(3.91988); m3_limit_T1tttt_1500_100.push_back(0.231445); m3_signif_T1tttt_1200_800.push_back(4.0305); m3_limit_T1tttt_1200_800.push_back(0.25293); 
m3_signif_T1tttt_1500_100.push_back(3.96638); m3_limit_T1tttt_1500_100.push_back(0.224609); m3_signif_T1tttt_1200_800.push_back(4.08135); m3_limit_T1tttt_1200_800.push_back(0.245117); 
m3_signif_T1tttt_1500_100.push_back(4.01006); m3_limit_T1tttt_1500_100.push_back(0.217773); m3_signif_T1tttt_1200_800.push_back(4.12928); m3_limit_T1tttt_1200_800.push_back(0.237305); 
m3_signif_T1tttt_1500_100.push_back(4.05119); m3_limit_T1tttt_1500_100.push_back(0.211914); m3_signif_T1tttt_1200_800.push_back(4.17455); m3_limit_T1tttt_1200_800.push_back(0.231445); 
    break;
  case kSystOnly:
m1_signif_T1tttt_1500_100.push_back(1.66516); m1_limit_T1tttt_1500_100.push_back(1.80469); m1_signif_T1tttt_1200_800.push_back(1.02639); m1_limit_T1tttt_1200_800.push_back(3.92188); 
m1_signif_T1tttt_1500_100.push_back(2.06396); m1_limit_T1tttt_1500_100.push_back(1.06641); m1_signif_T1tttt_1200_800.push_back(1.29384); m1_limit_T1tttt_1200_800.push_back(2.33594); 
m1_signif_T1tttt_1500_100.push_back(2.31243); m1_limit_T1tttt_1500_100.push_back(0.800781); m1_signif_T1tttt_1200_800.push_back(1.4641); m1_limit_T1tttt_1200_800.push_back(1.77344); 
m1_signif_T1tttt_1500_100.push_back(2.49609); m1_limit_T1tttt_1500_100.push_back(0.664062); m1_signif_T1tttt_1200_800.push_back(1.59257); m1_limit_T1tttt_1200_800.push_back(1.47266); 
m1_signif_T1tttt_1500_100.push_back(2.64269); m1_limit_T1tttt_1500_100.push_back(0.576172); m1_signif_T1tttt_1200_800.push_back(1.69728); m1_limit_T1tttt_1200_800.push_back(1.28516); 
m1_signif_T1tttt_1500_100.push_back(2.76497); m1_limit_T1tttt_1500_100.push_back(0.517578); m1_signif_T1tttt_1200_800.push_back(1.78637); m1_limit_T1tttt_1200_800.push_back(1.16016); 
m1_signif_T1tttt_1500_100.push_back(2.86994); m1_limit_T1tttt_1500_100.push_back(0.474609); m1_signif_T1tttt_1200_800.push_back(1.86425); m1_limit_T1tttt_1200_800.push_back(1.06641); 
m1_signif_T1tttt_1500_100.push_back(2.96191); m1_limit_T1tttt_1500_100.push_back(0.439453); m1_signif_T1tttt_1200_800.push_back(1.93362); m1_limit_T1tttt_1200_800.push_back(0.988281); 
m1_signif_T1tttt_1500_100.push_back(3.04376); m1_limit_T1tttt_1500_100.push_back(0.416016); m1_signif_T1tttt_1200_800.push_back(1.99625); m1_limit_T1tttt_1200_800.push_back(0.933594); 
m1_signif_T1tttt_1500_100.push_back(3.11745); m1_limit_T1tttt_1500_100.push_back(0.392578); m1_signif_T1tttt_1200_800.push_back(2.05339); m1_limit_T1tttt_1200_800.push_back(0.886719); 
m1_signif_T1tttt_1500_100.push_back(3.18444); m1_limit_T1tttt_1500_100.push_back(0.374023); m1_signif_T1tttt_1200_800.push_back(2.10592); m1_limit_T1tttt_1200_800.push_back(0.847656); 
m1_signif_T1tttt_1500_100.push_back(3.24586); m1_limit_T1tttt_1500_100.push_back(0.358398); m1_signif_T1tttt_1200_800.push_back(2.15457); m1_limit_T1tttt_1200_800.push_back(0.808594); 
m1_signif_T1tttt_1500_100.push_back(3.30251); m1_limit_T1tttt_1500_100.push_back(0.34668); m1_signif_T1tttt_1200_800.push_back(2.19988); m1_limit_T1tttt_1200_800.push_back(0.78125); 
m1_signif_T1tttt_1500_100.push_back(3.35506); m1_limit_T1tttt_1500_100.push_back(0.334961); m1_signif_T1tttt_1200_800.push_back(2.24223); m1_limit_T1tttt_1200_800.push_back(0.755859); 
m1_signif_T1tttt_1500_100.push_back(3.40407); m1_limit_T1tttt_1500_100.push_back(0.323242); m1_signif_T1tttt_1200_800.push_back(2.28201); m1_limit_T1tttt_1200_800.push_back(0.732422); 
m1_signif_T1tttt_1500_100.push_back(3.44996); m1_limit_T1tttt_1500_100.push_back(0.31543); m1_signif_T1tttt_1200_800.push_back(2.31952); m1_limit_T1tttt_1200_800.push_back(0.712891); 
m1_signif_T1tttt_1500_100.push_back(3.49308); m1_limit_T1tttt_1500_100.push_back(0.307617); m1_signif_T1tttt_1200_800.push_back(2.35495); m1_limit_T1tttt_1200_800.push_back(0.693359); 
m1_signif_T1tttt_1500_100.push_back(3.53371); m1_limit_T1tttt_1500_100.push_back(0.299805); m1_signif_T1tttt_1200_800.push_back(2.38853); m1_limit_T1tttt_1200_800.push_back(0.677734); 
m1_signif_T1tttt_1500_100.push_back(3.57216); m1_limit_T1tttt_1500_100.push_back(0.293945); m1_signif_T1tttt_1200_800.push_back(2.42043); m1_limit_T1tttt_1200_800.push_back(0.666016); 
m1_signif_T1tttt_1500_100.push_back(3.60861); m1_limit_T1tttt_1500_100.push_back(0.288086); m1_signif_T1tttt_1200_800.push_back(2.45082); m1_limit_T1tttt_1200_800.push_back(0.650391); 
m2_signif_T1tttt_1500_100.push_back(1.39567); m2_limit_T1tttt_1500_100.push_back(1.71094); m2_signif_T1tttt_1200_800.push_back(1.27629); m2_limit_T1tttt_1200_800.push_back(1.92969); 
m2_signif_T1tttt_1500_100.push_back(1.62589); m2_limit_T1tttt_1500_100.push_back(1.07422); m2_signif_T1tttt_1200_800.push_back(1.5281); m2_limit_T1tttt_1200_800.push_back(1.23047); 
m2_signif_T1tttt_1500_100.push_back(1.76146); m2_limit_T1tttt_1500_100.push_back(0.832031); m2_signif_T1tttt_1200_800.push_back(1.68372); m2_limit_T1tttt_1200_800.push_back(0.957031); 
m2_signif_T1tttt_1500_100.push_back(1.86004); m2_limit_T1tttt_1500_100.push_back(0.705078); m2_signif_T1tttt_1200_800.push_back(1.80039); m2_limit_T1tttt_1200_800.push_back(0.816406); 
m2_signif_T1tttt_1500_100.push_back(1.93833); m2_limit_T1tttt_1500_100.push_back(0.626953); m2_signif_T1tttt_1200_800.push_back(1.89526); m2_limit_T1tttt_1200_800.push_back(0.720703); 
m2_signif_T1tttt_1500_100.push_back(2.00356); m2_limit_T1tttt_1500_100.push_back(0.568359); m2_signif_T1tttt_1200_800.push_back(1.97588); m2_limit_T1tttt_1200_800.push_back(0.654297); 
m2_signif_T1tttt_1500_100.push_back(2.05955); m2_limit_T1tttt_1500_100.push_back(0.525391); m2_signif_T1tttt_1200_800.push_back(2.04629); m2_limit_T1tttt_1200_800.push_back(0.603516); 
m2_signif_T1tttt_1500_100.push_back(2.1086); m2_limit_T1tttt_1500_100.push_back(0.494141); m2_signif_T1tttt_1200_800.push_back(2.1089); m2_limit_T1tttt_1200_800.push_back(0.564453); 
m2_signif_T1tttt_1500_100.push_back(2.15221); m2_limit_T1tttt_1500_100.push_back(0.466797); m2_signif_T1tttt_1200_800.push_back(2.16535); m2_limit_T1tttt_1200_800.push_back(0.533203); 
m2_signif_T1tttt_1500_100.push_back(2.19145); m2_limit_T1tttt_1500_100.push_back(0.443359); m2_signif_T1tttt_1200_800.push_back(2.21678); m2_limit_T1tttt_1200_800.push_back(0.505859); 
m2_signif_T1tttt_1500_100.push_back(2.22705); m2_limit_T1tttt_1500_100.push_back(0.427734); m2_signif_T1tttt_1200_800.push_back(2.26397); m2_limit_T1tttt_1200_800.push_back(0.486328); 
m2_signif_T1tttt_1500_100.push_back(2.2596); m2_limit_T1tttt_1500_100.push_back(0.412109); m2_signif_T1tttt_1200_800.push_back(2.30758); m2_limit_T1tttt_1200_800.push_back(0.466797); 
m2_signif_T1tttt_1500_100.push_back(2.28955); m2_limit_T1tttt_1500_100.push_back(0.396484); m2_signif_T1tttt_1200_800.push_back(2.34809); m2_limit_T1tttt_1200_800.push_back(0.451172); 
m2_signif_T1tttt_1500_100.push_back(2.31723); m2_limit_T1tttt_1500_100.push_back(0.383789); m2_signif_T1tttt_1200_800.push_back(2.38588); m2_limit_T1tttt_1200_800.push_back(0.435547); 
m2_signif_T1tttt_1500_100.push_back(2.34296); m2_limit_T1tttt_1500_100.push_back(0.374023); m2_signif_T1tttt_1200_800.push_back(2.42126); m2_limit_T1tttt_1200_800.push_back(0.423828); 
m2_signif_T1tttt_1500_100.push_back(2.36694); m2_limit_T1tttt_1500_100.push_back(0.364258); m2_signif_T1tttt_1200_800.push_back(2.45452); m2_limit_T1tttt_1200_800.push_back(0.412109); 
m2_signif_T1tttt_1500_100.push_back(2.38938); m2_limit_T1tttt_1500_100.push_back(0.356445); m2_signif_T1tttt_1200_800.push_back(2.48588); m2_limit_T1tttt_1200_800.push_back(0.404297); 
m2_signif_T1tttt_1500_100.push_back(2.41043); m2_limit_T1tttt_1500_100.push_back(0.348633); m2_signif_T1tttt_1200_800.push_back(2.51547); m2_limit_T1tttt_1200_800.push_back(0.392578); 
m2_signif_T1tttt_1500_100.push_back(2.43025); m2_limit_T1tttt_1500_100.push_back(0.34082); m2_signif_T1tttt_1200_800.push_back(2.54354); m2_limit_T1tttt_1200_800.push_back(0.385742); 
m2_signif_T1tttt_1500_100.push_back(2.44895); m2_limit_T1tttt_1500_100.push_back(0.334961); m2_signif_T1tttt_1200_800.push_back(2.57018); m2_limit_T1tttt_1200_800.push_back(0.37793); 
m3_signif_T1tttt_1500_100.push_back(1.43494); m3_limit_T1tttt_1500_100.push_back(1.66406); m3_signif_T1tttt_1200_800.push_back(1.38785); m3_limit_T1tttt_1200_800.push_back(1.82031); 
m3_signif_T1tttt_1500_100.push_back(1.68383); m3_limit_T1tttt_1500_100.push_back(1.05078); m3_signif_T1tttt_1200_800.push_back(1.67508); m3_limit_T1tttt_1200_800.push_back(1.15234); 
m3_signif_T1tttt_1500_100.push_back(1.83454); m3_limit_T1tttt_1500_100.push_back(0.816406); m3_signif_T1tttt_1200_800.push_back(1.85391); m3_limit_T1tttt_1200_800.push_back(0.894531); 
m3_signif_T1tttt_1500_100.push_back(1.94577); m3_limit_T1tttt_1500_100.push_back(0.693359); m3_signif_T1tttt_1200_800.push_back(1.98669); m3_limit_T1tttt_1200_800.push_back(0.755859); 
m3_signif_T1tttt_1500_100.push_back(2.03481); m3_limit_T1tttt_1500_100.push_back(0.615234); m3_signif_T1tttt_1200_800.push_back(2.09297); m3_limit_T1tttt_1200_800.push_back(0.669922); 
m3_signif_T1tttt_1500_100.push_back(2.10933); m3_limit_T1tttt_1500_100.push_back(0.560547); m3_signif_T1tttt_1200_800.push_back(2.18173); m3_limit_T1tttt_1200_800.push_back(0.607422); 
m3_signif_T1tttt_1500_100.push_back(2.17343); m3_limit_T1tttt_1500_100.push_back(0.517578); m3_signif_T1tttt_1200_800.push_back(2.2579); m3_limit_T1tttt_1200_800.push_back(0.560547); 
m3_signif_T1tttt_1500_100.push_back(2.22963); m3_limit_T1tttt_1500_100.push_back(0.486328); m3_signif_T1tttt_1200_800.push_back(2.32455); m3_limit_T1tttt_1200_800.push_back(0.525391); 
m3_signif_T1tttt_1500_100.push_back(2.27962); m3_limit_T1tttt_1500_100.push_back(0.458984); m3_signif_T1tttt_1200_800.push_back(2.3837); m3_limit_T1tttt_1200_800.push_back(0.498047); 
m3_signif_T1tttt_1500_100.push_back(2.32456); m3_limit_T1tttt_1500_100.push_back(0.439453); m3_signif_T1tttt_1200_800.push_back(2.43683); m3_limit_T1tttt_1200_800.push_back(0.474609); 
m3_signif_T1tttt_1500_100.push_back(2.36533); m3_limit_T1tttt_1500_100.push_back(0.419922); m3_signif_T1tttt_1200_800.push_back(2.48496); m3_limit_T1tttt_1200_800.push_back(0.451172); 
m3_signif_T1tttt_1500_100.push_back(2.40256); m3_limit_T1tttt_1500_100.push_back(0.404297); m3_signif_T1tttt_1200_800.push_back(2.52889); m3_limit_T1tttt_1200_800.push_back(0.435547); 
m3_signif_T1tttt_1500_100.push_back(2.43678); m3_limit_T1tttt_1500_100.push_back(0.392578); m3_signif_T1tttt_1200_800.push_back(2.56926); m3_limit_T1tttt_1200_800.push_back(0.419922); 
m3_signif_T1tttt_1500_100.push_back(2.46838); m3_limit_T1tttt_1500_100.push_back(0.379883); m3_signif_T1tttt_1200_800.push_back(2.60655); m3_limit_T1tttt_1200_800.push_back(0.408203); 
m3_signif_T1tttt_1500_100.push_back(2.49771); m3_limit_T1tttt_1500_100.push_back(0.370117); m3_signif_T1tttt_1200_800.push_back(2.64116); m3_limit_T1tttt_1200_800.push_back(0.396484); 
m3_signif_T1tttt_1500_100.push_back(2.52502); m3_limit_T1tttt_1500_100.push_back(0.360352); m3_signif_T1tttt_1200_800.push_back(2.67343); m3_limit_T1tttt_1200_800.push_back(0.387695); 
m3_signif_T1tttt_1500_100.push_back(2.55055); m3_limit_T1tttt_1500_100.push_back(0.352539); m3_signif_T1tttt_1200_800.push_back(2.70361); m3_limit_T1tttt_1200_800.push_back(0.37793); 
m3_signif_T1tttt_1500_100.push_back(2.57447); m3_limit_T1tttt_1500_100.push_back(0.344727); m3_signif_T1tttt_1200_800.push_back(2.73192); m3_limit_T1tttt_1200_800.push_back(0.370117); 
m3_signif_T1tttt_1500_100.push_back(2.59697); m3_limit_T1tttt_1500_100.push_back(0.336914); m3_signif_T1tttt_1200_800.push_back(2.75858); m3_limit_T1tttt_1200_800.push_back(0.362305); 
m3_signif_T1tttt_1500_100.push_back(2.61818); m3_limit_T1tttt_1500_100.push_back(0.331055); m3_signif_T1tttt_1200_800.push_back(2.78375); m3_limit_T1tttt_1200_800.push_back(0.356445); 
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
    }else{
      ncz.at(i) = 4.*ncz.at(i);
    }
    if(cz.at(i) <= 0.){
      cz.at(i) = big_num;
    }else{
      cz.at(i) = 4.*cz.at(i);
    }
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
  TH1D h("h", ";Luminosity [fb^{-1}];Expected Significance", 1, 0., 15.);
  h.SetMaximum(8.);
  TAxis &yaxis = *h.GetYaxis();
  yaxis.SetLabelColor(kRed);
  yaxis.SetTitleColor(kRed);
  yaxis.SetTitleOffset(.8);
  TCanvas c;
  TGaxis *raxis = new TGaxis(15., 0.,
                             15., 8.,
                             0, 2., 510, "+L");
  raxis->SetLabelColor(kBlue);
  raxis->SetTitleColor(kBlue);
  raxis->SetTitle("Expected Limit/X-Section");
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
  TLegend l(0.12, 0.65, 0.5, 0.9);
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
