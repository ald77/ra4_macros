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
  case kStatOnly:
    m1_signif_T1tttt_1500_100.push_back(2.32098); m1_limit_T1tttt_1500_100.push_back(0.589862620996); m1_signif_T1tttt_1200_800.push_back(1.5839); m1_limit_T1tttt_1200_800.push_back(0.267782068242); 
    m1_signif_T1tttt_1500_100.push_back(2.83183); m1_limit_T1tttt_1500_100.push_back(1.07112942028); m1_signif_T1tttt_1200_800.push_back(1.98274); m1_limit_T1tttt_1200_800.push_back(0.479400174502); 
    m1_signif_T1tttt_1500_100.push_back(3.11812); m1_limit_T1tttt_1500_100.push_back(1.51032533918); m1_signif_T1tttt_1200_800.push_back(2.21269); m1_limit_T1tttt_1200_800.push_back(0.66840898609); 
    m1_signif_T1tttt_1500_100.push_back(3.31691); m1_limit_T1tttt_1500_100.push_back(1.90334436639); m1_signif_T1tttt_1200_800.push_back(2.37045); m1_limit_T1tttt_1200_800.push_back(0.844887165319); 
    m1_signif_T1tttt_1500_100.push_back(3.46999); m1_limit_T1tttt_1500_100.push_back(2.27555620282); m1_signif_T1tttt_1200_800.push_back(2.48954); m1_limit_T1tttt_1200_800.push_back(1.00392131666); 
    m1_signif_T1tttt_1500_100.push_back(3.5948); m1_limit_T1tttt_1500_100.push_back(2.61224830988); m1_signif_T1tttt_1200_800.push_back(2.58497); m1_limit_T1tttt_1200_800.push_back(1.15837137618); 
    m1_signif_T1tttt_1500_100.push_back(3.70025); m1_limit_T1tttt_1500_100.push_back(2.9341001115); m1_signif_T1tttt_1200_800.push_back(2.66452); m1_limit_T1tttt_1200_800.push_back(1.30279919435); 
    m1_signif_T1tttt_1500_100.push_back(3.7915); m1_limit_T1tttt_1500_100.push_back(3.23028717253); m1_signif_T1tttt_1200_800.push_back(2.73269); m1_limit_T1tttt_1200_800.push_back(1.43417289815); 
    m1_signif_T1tttt_1500_100.push_back(3.87184); m1_limit_T1tttt_1500_100.push_back(3.50685238957); m1_signif_T1tttt_1200_800.push_back(2.79229); m1_limit_T1tttt_1200_800.push_back(1.56574892903); 
    m1_signif_T1tttt_1500_100.push_back(3.94349); m1_limit_T1tttt_1500_100.push_back(3.77860403253); m1_signif_T1tttt_1200_800.push_back(2.84519); m1_limit_T1tttt_1200_800.push_back(1.68976861998); 
    m1_signif_T1tttt_1500_100.push_back(4.00808); m1_limit_T1tttt_1500_100.push_back(4.01569332953); m1_signif_T1tttt_1200_800.push_back(2.8927); m1_limit_T1tttt_1200_800.push_back(1.7964900178); 
    m1_signif_T1tttt_1500_100.push_back(4.06679); m1_limit_T1tttt_1500_100.push_back(4.24895475713); m1_signif_T1tttt_1200_800.push_back(2.93577); m1_limit_T1tttt_1200_800.push_back(1.91760437521); 
    m1_signif_T1tttt_1500_100.push_back(4.12052); m1_limit_T1tttt_1500_100.push_back(4.4716119714); m1_signif_T1tttt_1200_800.push_back(2.97513); m1_limit_T1tttt_1200_800.push_back(2.02371387924); 
    m1_signif_T1tttt_1500_100.push_back(4.16996); m1_limit_T1tttt_1500_100.push_back(4.67580318609); m1_signif_T1tttt_1200_800.push_back(3.01132); m1_limit_T1tttt_1200_800.push_back(2.12448189198); 
    m1_signif_T1tttt_1500_100.push_back(4.2157); m1_limit_T1tttt_1500_100.push_back(4.8995110288); m1_signif_T1tttt_1200_800.push_back(3.04476); m1_limit_T1tttt_1200_800.push_back(2.21644960237); 
    m1_signif_T1tttt_1500_100.push_back(4.25821); m1_limit_T1tttt_1500_100.push_back(5.09452898532); m1_signif_T1tttt_1200_800.push_back(3.07584); m1_limit_T1tttt_1200_800.push_back(2.30630497654); 
    m1_signif_T1tttt_1500_100.push_back(4.29784); m1_limit_T1tttt_1500_100.push_back(5.26479275143); m1_signif_T1tttt_1200_800.push_back(3.10481); m1_limit_T1tttt_1200_800.push_back(2.40375370178); 
    m1_signif_T1tttt_1500_100.push_back(4.33492); m1_limit_T1tttt_1500_100.push_back(5.46134731438); m1_signif_T1tttt_1200_800.push_back(3.13192); m1_limit_T1tttt_1200_800.push_back(2.47342918696); 
    m1_signif_T1tttt_1500_100.push_back(4.36972); m1_limit_T1tttt_1500_100.push_back(5.6109480819); m1_signif_T1tttt_1200_800.push_back(3.15737); m1_limit_T1tttt_1200_800.push_back(2.56); 
    m1_signif_T1tttt_1500_100.push_back(4.40248); m1_limit_T1tttt_1500_100.push_back(5.78529609145); m1_signif_T1tttt_1200_800.push_back(3.18134); m1_limit_T1tttt_1200_800.push_back(2.64599264414); 
    m2_signif_T1tttt_1500_100.push_back(1.90902); m2_limit_T1tttt_1500_100.push_back(0.654733065329); m2_signif_T1tttt_1200_800.push_back(1.68514); m2_limit_T1tttt_1200_800.push_back(0.540085117415); 
    m2_signif_T1tttt_1500_100.push_back(2.36308); m2_limit_T1tttt_1500_100.push_back(1.10822480118); m2_signif_T1tttt_1200_800.push_back(2.16138); m2_limit_T1tttt_1200_800.push_back(0.891989046375); 
    m2_signif_T1tttt_1500_100.push_back(2.64134); m2_limit_T1tttt_1500_100.push_back(1.48405824632); m2_signif_T1tttt_1200_800.push_back(2.46678); m2_limit_T1tttt_1200_800.push_back(1.17972385024); 
    m2_signif_T1tttt_1500_100.push_back(2.84561); m2_limit_T1tttt_1500_100.push_back(1.80281527634); m2_signif_T1tttt_1200_800.push_back(2.69431); m2_limit_T1tttt_1200_800.push_back(1.43417289815); 
    m2_signif_T1tttt_1500_100.push_back(3.0083); m2_limit_T1tttt_1500_100.push_back(2.08979428065); m2_signif_T1tttt_1200_800.push_back(2.87635); m2_limit_T1tttt_1200_800.push_back(1.65695689924); 
    m2_signif_T1tttt_1500_100.push_back(3.1439); m2_limit_T1tttt_1500_100.push_back(2.33790159305); m2_signif_T1tttt_1200_800.push_back(3.02816); m2_limit_T1tttt_1200_800.push_back(1.86181948171); 
    m2_signif_T1tttt_1500_100.push_back(3.2602); m2_limit_T1tttt_1500_100.push_back(2.57934716723); m2_signif_T1tttt_1200_800.push_back(3.15823); m2_limit_T1tttt_1200_800.push_back(2.05622542811); 
    m2_signif_T1tttt_1500_100.push_back(3.36197); m2_limit_T1tttt_1500_100.push_back(2.80548191166); m2_signif_T1tttt_1200_800.push_back(3.27181); m2_limit_T1tttt_1200_800.push_back(2.2358059857); 
    m2_signif_T1tttt_1500_100.push_back(3.4523); m2_limit_T1tttt_1500_100.push_back(3.00293086052); m2_signif_T1tttt_1200_800.push_back(3.37239); m2_limit_T1tttt_1200_800.push_back(2.40375370178); 
    m2_signif_T1tttt_1500_100.push_back(3.53338); m2_limit_T1tttt_1500_100.push_back(3.19002670052); m2_signif_T1tttt_1200_800.push_back(3.46247); m2_limit_T1tttt_1200_800.push_back(2.56); 
    m2_signif_T1tttt_1500_100.push_back(3.60682); m2_limit_T1tttt_1500_100.push_back(3.37954295061); m2_signif_T1tttt_1200_800.push_back(3.54385); m2_limit_T1tttt_1200_800.push_back(2.71618083246); 
    m2_signif_T1tttt_1500_100.push_back(3.67379); m2_limit_T1tttt_1500_100.push_back(3.54324710251); m2_signif_T1tttt_1200_800.push_back(3.61789); m2_limit_T1tttt_1200_800.push_back(2.85236717952); 
    m2_signif_T1tttt_1500_100.push_back(3.73528); m2_limit_T1tttt_1500_100.push_back(3.72363203068); m2_signif_T1tttt_1200_800.push_back(3.68569); m2_limit_T1tttt_1200_800.push_back(2.98542218348); 
    m2_signif_T1tttt_1500_100.push_back(3.79197); m2_limit_T1tttt_1500_100.push_back(3.86415187663); m2_signif_T1tttt_1200_800.push_back(3.74809); m2_limit_T1tttt_1200_800.push_back(3.11246261154); 
    m2_signif_T1tttt_1500_100.push_back(3.84439); m2_limit_T1tttt_1500_100.push_back(4.01569332953); m2_signif_T1tttt_1200_800.push_back(3.80578); m2_limit_T1tttt_1200_800.push_back(3.23028717253); 
    m2_signif_T1tttt_1500_100.push_back(3.8929); m2_limit_T1tttt_1500_100.push_back(4.14574791365); m2_signif_T1tttt_1200_800.push_back(3.85932); m2_limit_T1tttt_1200_800.push_back(3.3573721177); 
    m2_signif_T1tttt_1500_100.push_back(3.93799); m2_limit_T1tttt_1500_100.push_back(4.2845268597); m2_signif_T1tttt_1200_800.push_back(3.90921); m2_limit_T1tttt_1200_800.push_back(3.47118568761); 
    m2_signif_T1tttt_1500_100.push_back(3.97999); m2_limit_T1tttt_1500_100.push_back(4.43289920474); m2_signif_T1tttt_1200_800.push_back(3.95579); m2_limit_T1tttt_1200_800.push_back(3.56794982035); 
    m2_signif_T1tttt_1500_100.push_back(4.01928); m2_limit_T1tttt_1500_100.push_back(4.55110204936); m2_signif_T1tttt_1200_800.push_back(3.99948); m2_limit_T1tttt_1200_800.push_back(3.68345832535); 
    m2_signif_T1tttt_1500_100.push_back(4.05613); m2_limit_T1tttt_1500_100.push_back(4.67580318609); m2_signif_T1tttt_1200_800.push_back(4.04053); m2_limit_T1tttt_1200_800.push_back(3.77860403253); 
    m3_signif_T1tttt_1500_100.push_back(1.97357); m3_limit_T1tttt_1500_100.push_back(0.686327666555); m3_signif_T1tttt_1200_800.push_back(1.89814); m3_limit_T1tttt_1200_800.push_back(0.606634151076); 
    m3_signif_T1tttt_1500_100.push_back(2.45731); m3_limit_T1tttt_1500_100.push_back(1.15837137618); m3_signif_T1tttt_1200_800.push_back(2.45206); m3_limit_T1tttt_1200_800.push_back(1.02811271405); 
    m3_signif_T1tttt_1500_100.push_back(2.75858); m3_limit_T1tttt_1500_100.push_back(1.54682869182); m3_signif_T1tttt_1200_800.push_back(2.81164); m3_limit_T1tttt_1200_800.push_back(1.38005462256); 
    m3_signif_T1tttt_1500_100.push_back(2.98177); m3_limit_T1tttt_1500_100.push_back(1.87545831513); m3_signif_T1tttt_1200_800.push_back(3.08031); m3_limit_T1tttt_1200_800.push_back(1.67868887684); 
    m3_signif_T1tttt_1500_100.push_back(3.16046); m3_limit_T1tttt_1500_100.push_back(2.1603358026); m3_signif_T1tttt_1200_800.push_back(3.29491); m3_limit_T1tttt_1200_800.push_back(1.96168438068); 
    m3_signif_T1tttt_1500_100.push_back(3.30985); m3_limit_T1tttt_1500_100.push_back(2.4265424924); m3_signif_T1tttt_1200_800.push_back(3.47318); m3_limit_T1tttt_1200_800.push_back(2.21644960237); 
    m3_signif_T1tttt_1500_100.push_back(3.43821); m3_limit_T1tttt_1500_100.push_back(2.6736323702); m3_signif_T1tttt_1200_800.push_back(3.62515); m3_limit_T1tttt_1200_800.push_back(2.43809672417); 
    m3_signif_T1tttt_1500_100.push_back(3.55061); m3_limit_T1tttt_1500_100.push_back(2.90084617683); m3_signif_T1tttt_1200_800.push_back(3.75716); m3_limit_T1tttt_1200_800.push_back(2.64599264414); 
    m3_signif_T1tttt_1500_100.push_back(3.65041); m3_limit_T1tttt_1500_100.push_back(3.09365738363); m3_signif_T1tttt_1200_800.push_back(3.87345); m3_limit_T1tttt_1200_800.push_back(2.85236717952); 
    m3_signif_T1tttt_1500_100.push_back(3.73999); m3_limit_T1tttt_1500_100.push_back(3.29260382403); m3_signif_T1tttt_1200_800.push_back(3.97704); m3_limit_T1tttt_1200_800.push_back(3.03857162825); 
    m3_signif_T1tttt_1500_100.push_back(3.82109); m3_limit_T1tttt_1500_100.push_back(3.47118568761); m3_signif_T1tttt_1200_800.push_back(4.07014); m3_limit_T1tttt_1200_800.push_back(3.2100358561); 
    m3_signif_T1tttt_1500_100.push_back(3.89501); m3_limit_T1tttt_1500_100.push_back(3.64412894386); m3_signif_T1tttt_1200_800.push_back(4.15445); m3_limit_T1tttt_1200_800.push_back(3.37954295061); 
    m3_signif_T1tttt_1500_100.push_back(3.96283); m3_limit_T1tttt_1500_100.push_back(3.80669597823); m3_signif_T1tttt_1200_800.push_back(4.23131); m3_limit_T1tttt_1200_800.push_back(3.51889647407); 
    m3_signif_T1tttt_1500_100.push_back(4.02533); m3_limit_T1tttt_1500_100.push_back(3.98442885205); m3_signif_T1tttt_1200_800.push_back(4.30174); m3_limit_T1tttt_1200_800.push_back(3.67025005414); 
    m3_signif_T1tttt_1500_100.push_back(4.08309); m3_limit_T1tttt_1500_100.push_back(4.11245085621); m3_signif_T1tttt_1200_800.push_back(4.36659); m3_limit_T1tttt_1200_800.push_back(3.80669597823); 
    m3_signif_T1tttt_1500_100.push_back(4.13653); m3_limit_T1tttt_1500_100.push_back(4.24895475713); m3_signif_T1tttt_1200_800.push_back(4.42657); m3_limit_T1tttt_1200_800.push_back(3.95366306883); 
    m3_signif_T1tttt_1500_100.push_back(4.1862); m3_limit_T1tttt_1500_100.push_back(4.39485099258); m3_signif_T1tttt_1200_800.push_back(4.48224); m3_limit_T1tttt_1200_800.push_back(4.07968439562); 
    m3_signif_T1tttt_1500_100.push_back(4.23246); m3_limit_T1tttt_1500_100.push_back(4.55110204936); m3_signif_T1tttt_1200_800.push_back(4.53408); m3_limit_T1tttt_1200_800.push_back(4.21398622027); 
    m3_signif_T1tttt_1500_100.push_back(4.27572); m3_limit_T1tttt_1500_100.push_back(4.67580318609); m3_signif_T1tttt_1200_800.push_back(4.58252); m3_limit_T1tttt_1200_800.push_back(4.32068093932); 
    m3_signif_T1tttt_1500_100.push_back(4.31629); m3_limit_T1tttt_1500_100.push_back(4.76278927991); m3_signif_T1tttt_1200_800.push_back(4.62791); m3_limit_T1tttt_1200_800.push_back(4.43289920474);
    break;
  case kAll:
    //Systematics
    m1_signif_T1tttt_1500_100.push_back(2.05316); m1_limit_T1tttt_1500_100.push_back(0.584474031819); m1_signif_T1tttt_1200_800.push_back(1.50163); m1_limit_T1tttt_1200_800.push_back(0.265560518587); 
    m1_signif_T1tttt_1500_100.push_back(2.27528); m1_limit_T1tttt_1500_100.push_back(1.05349766492); m1_signif_T1tttt_1200_800.push_back(1.75775); m1_limit_T1tttt_1200_800.push_back(0.472324165521); 
    m1_signif_T1tttt_1500_100.push_back(2.368); m1_limit_T1tttt_1500_100.push_back(1.47550513918); m1_signif_T1tttt_1200_800.push_back(1.86711); m1_limit_T1tttt_1200_800.push_back(0.658098227741); 
    m1_signif_T1tttt_1500_100.push_back(2.42313); m1_limit_T1tttt_1500_100.push_back(1.84837417008); m1_signif_T1tttt_1200_800.push_back(1.92981); m1_limit_T1tttt_1200_800.push_back(0.823153666326); 
    m1_signif_T1tttt_1500_100.push_back(2.46113); m1_limit_T1tttt_1500_100.push_back(2.17872518432); m1_signif_T1tttt_1200_800.push_back(1.97191); m1_limit_T1tttt_1200_800.push_back(0.973387583468); 
    m1_signif_T1tttt_1500_100.push_back(2.4895); m1_limit_T1tttt_1500_100.push_back(2.47342918696); m1_signif_T1tttt_1200_800.push_back(2.00289); m1_limit_T1tttt_1200_800.push_back(1.10822480118); 
    m1_signif_T1tttt_1500_100.push_back(2.51178); m1_limit_T1tttt_1500_100.push_back(2.76010543603); m1_signif_T1tttt_1200_800.push_back(2.02704); m1_limit_T1tttt_1200_800.push_back(1.23671459348); 
    m1_signif_T1tttt_1500_100.push_back(2.52989); m1_limit_T1tttt_1500_100.push_back(3.0206461162); m1_signif_T1tttt_1200_800.push_back(2.04665); m1_limit_T1tttt_1200_800.push_back(1.35809041623); 
    m1_signif_T1tttt_1500_100.push_back(2.54498); m1_limit_T1tttt_1500_100.push_back(3.25079563223); m1_signif_T1tttt_1200_800.push_back(2.06302); m1_limit_T1tttt_1200_800.push_back(1.47550513918); 
    m1_signif_T1tttt_1500_100.push_back(2.5578); m1_limit_T1tttt_1500_100.push_back(3.47118568761); m1_signif_T1tttt_1200_800.push_back(2.07699); m1_limit_T1tttt_1200_800.push_back(1.5753836847); 
    m1_signif_T1tttt_1500_100.push_back(2.56887); m1_limit_T1tttt_1500_100.push_back(3.67025005414); m1_signif_T1tttt_1200_800.push_back(2.08911); m1_limit_T1tttt_1200_800.push_back(1.67868887684); 
    m1_signif_T1tttt_1500_100.push_back(2.57855); m1_limit_T1tttt_1500_100.push_back(3.86415187663); m1_signif_T1tttt_1200_800.push_back(2.09977); m1_limit_T1tttt_1200_800.push_back(1.77162668991); 
    m1_signif_T1tttt_1500_100.push_back(2.5871); m1_limit_T1tttt_1500_100.push_back(4.04743594933); m1_signif_T1tttt_1200_800.push_back(2.10926); m1_limit_T1tttt_1200_800.push_back(1.86181948171); 
    m1_signif_T1tttt_1500_100.push_back(2.59471); m1_limit_T1tttt_1500_100.push_back(4.21398622027); m1_signif_T1tttt_1200_800.push_back(2.11777); m1_limit_T1tttt_1200_800.push_back(1.9467675871); 
    m1_signif_T1tttt_1500_100.push_back(2.60155); m1_limit_T1tttt_1500_100.push_back(4.39485099258); m1_signif_T1tttt_1200_800.push_back(2.12547); m1_limit_T1tttt_1200_800.push_back(2.02371387924); 
    m1_signif_T1tttt_1500_100.push_back(2.60773); m1_limit_T1tttt_1500_100.push_back(4.55110204936); m1_signif_T1tttt_1200_800.push_back(2.13248); m1_limit_T1tttt_1200_800.push_back(2.10699754956); 
    m1_signif_T1tttt_1500_100.push_back(2.61335); m1_limit_T1tttt_1500_100.push_back(4.67580318609); m1_signif_T1tttt_1200_800.push_back(2.1389); m1_limit_T1tttt_1200_800.push_back(2.17872518432); 
    m1_signif_T1tttt_1500_100.push_back(2.61848); m1_limit_T1tttt_1500_100.push_back(4.85307320861); m1_signif_T1tttt_1200_800.push_back(2.14481); m1_limit_T1tttt_1200_800.push_back(2.25550851567); 
    m1_signif_T1tttt_1500_100.push_back(2.62319); m1_limit_T1tttt_1500_100.push_back(4.94687060965); m1_signif_T1tttt_1200_800.push_back(2.15027); m1_limit_T1tttt_1200_800.push_back(2.31674006871); 
    m1_signif_T1tttt_1500_100.push_back(2.62753); m1_limit_T1tttt_1500_100.push_back(5.09452898532); m1_signif_T1tttt_1200_800.push_back(2.15535); m1_limit_T1tttt_1200_800.push_back(2.39252193344); 
    m2_signif_T1tttt_1500_100.push_back(1.65413); m2_limit_T1tttt_1500_100.push_back(0.641605553738); m2_signif_T1tttt_1200_800.push_back(1.49242); m2_limit_T1tttt_1200_800.push_back(0.531121037173); 
    m2_signif_T1tttt_1500_100.push_back(1.88494); m2_limit_T1tttt_1500_100.push_back(1.07112942028); m2_signif_T1tttt_1200_800.push_back(1.76451); m2_limit_T1tttt_1200_800.push_back(0.85618637465); 
    m2_signif_T1tttt_1500_100.push_back(1.99917); m2_limit_T1tttt_1500_100.push_back(1.41046906559); m2_signif_T1tttt_1200_800.push_back(1.90525); m2_limit_T1tttt_1200_800.push_back(1.11790424256); 
    m2_signif_T1tttt_1500_100.push_back(2.07314); m2_limit_T1tttt_1500_100.push_back(1.68976861998); m2_signif_T1tttt_1200_800.push_back(1.99688); m2_limit_T1tttt_1200_800.push_back(1.33681439803); 
    m2_signif_T1tttt_1500_100.push_back(2.12708); m2_limit_T1tttt_1500_100.push_back(1.93207593831); m2_signif_T1tttt_1200_800.push_back(2.06337); m2_limit_T1tttt_1200_800.push_back(1.53753665103); 
    m2_signif_T1tttt_1500_100.push_back(2.16914); m2_limit_T1tttt_1500_100.push_back(2.1603358026); m2_signif_T1tttt_1200_800.push_back(2.11477); m2_limit_T1tttt_1200_800.push_back(1.71237568153); 
    m2_signif_T1tttt_1500_100.push_back(2.20333); m2_limit_T1tttt_1500_100.push_back(2.35944770048); m2_signif_T1tttt_1200_800.push_back(2.15617); m2_limit_T1tttt_1200_800.push_back(1.87545831513); 
    m2_signif_T1tttt_1500_100.push_back(2.23195); m2_limit_T1tttt_1500_100.push_back(2.53465507146); m2_signif_T1tttt_1200_800.push_back(2.19051); m2_limit_T1tttt_1200_800.push_back(2.02371387924); 
    m2_signif_T1tttt_1500_100.push_back(2.25643); m2_limit_T1tttt_1500_100.push_back(2.70184833445); m2_signif_T1tttt_1200_800.push_back(2.21962); m2_limit_T1tttt_1200_800.push_back(2.1603358026); 
    m2_signif_T1tttt_1500_100.push_back(2.2777); m2_limit_T1tttt_1500_100.push_back(2.8683457963); m2_signif_T1tttt_1200_800.push_back(2.24472); m2_limit_T1tttt_1200_800.push_back(2.29596346663); 
    m2_signif_T1tttt_1500_100.push_back(2.29642); m2_limit_T1tttt_1500_100.push_back(3.00293086052); m2_signif_T1tttt_1200_800.push_back(2.26666); m2_limit_T1tttt_1200_800.push_back(2.4265424924); 
    m2_signif_T1tttt_1500_100.push_back(2.31307); m2_limit_T1tttt_1500_100.push_back(3.15076736939); m2_signif_T1tttt_1200_800.push_back(2.28605); m2_limit_T1tttt_1200_800.push_back(2.54726449266); 
    m2_signif_T1tttt_1500_100.push_back(2.32802); m2_limit_T1tttt_1500_100.push_back(3.27156616415); m2_signif_T1tttt_1200_800.push_back(2.30335); m2_limit_T1tttt_1200_800.push_back(2.64599264414); 
    m2_signif_T1tttt_1500_100.push_back(2.34151); m2_limit_T1tttt_1500_100.push_back(3.40199697222); m2_signif_T1tttt_1200_800.push_back(2.31892); m2_limit_T1tttt_1200_800.push_back(2.76010543603); 
    m2_signif_T1tttt_1500_100.push_back(2.35379); m2_limit_T1tttt_1500_100.push_back(3.51889647407); m2_signif_T1tttt_1200_800.push_back(2.33301); m2_limit_T1tttt_1200_800.push_back(2.8683457963); 
    m2_signif_T1tttt_1500_100.push_back(2.365); m2_limit_T1tttt_1500_100.push_back(3.64412894386); m2_signif_T1tttt_1200_800.push_back(2.34584); m2_limit_T1tttt_1200_800.push_back(2.96811649264); 
    m2_signif_T1tttt_1500_100.push_back(2.37531); m2_limit_T1tttt_1500_100.push_back(3.75090959558); m2_signif_T1tttt_1200_800.push_back(2.3576); m2_limit_T1tttt_1200_800.push_back(3.0567205057); 
    m2_signif_T1tttt_1500_100.push_back(2.3848); m2_limit_T1tttt_1500_100.push_back(3.86415187663); m2_signif_T1tttt_1200_800.push_back(2.3684); m2_limit_T1tttt_1200_800.push_back(3.15076736939); 
    m2_signif_T1tttt_1500_100.push_back(2.3936); m2_limit_T1tttt_1500_100.push_back(3.95366306883); m2_signif_T1tttt_1200_800.push_back(2.37839); m2_limit_T1tttt_1200_800.push_back(3.23028717253); 
    m2_signif_T1tttt_1500_100.push_back(2.40176); m2_limit_T1tttt_1500_100.push_back(4.07968439562); m2_signif_T1tttt_1200_800.push_back(2.38765); m2_limit_T1tttt_1200_800.push_back(3.31391379847); 
    m3_signif_T1tttt_1500_100.push_back(1.69155); m3_limit_T1tttt_1500_100.push_back(0.671916574838); m3_signif_T1tttt_1200_800.push_back(1.65515); m3_limit_T1tttt_1200_800.push_back(0.592592592593); 
    m3_signif_T1tttt_1500_100.push_back(1.92881); m3_limit_T1tttt_1500_100.push_back(1.10822480118); m3_signif_T1tttt_1200_800.push_back(1.95688); m3_limit_T1tttt_1200_800.push_back(0.984620231977); 
    m3_signif_T1tttt_1500_100.push_back(2.04765); m3_limit_T1tttt_1500_100.push_back(1.45868919272); m3_signif_T1tttt_1200_800.push_back(2.11376); m3_limit_T1tttt_1200_800.push_back(1.30279919435); 
    m3_signif_T1tttt_1500_100.push_back(2.12522); m3_limit_T1tttt_1500_100.push_back(1.7355928438); m3_signif_T1tttt_1200_800.push_back(2.21538); m3_limit_T1tttt_1200_800.push_back(1.56574892903); 
    m3_signif_T1tttt_1500_100.push_back(2.18209); m3_limit_T1tttt_1500_100.push_back(1.99221839495); m3_signif_T1tttt_1200_800.push_back(2.28838); m3_limit_T1tttt_1200_800.push_back(1.80281527634); 
    m3_signif_T1tttt_1500_100.push_back(2.2266); m3_limit_T1tttt_1500_100.push_back(2.19742549629); m3_signif_T1tttt_1200_800.push_back(2.34412); m3_limit_T1tttt_1200_800.push_back(2.00784263333); 
    m3_signif_T1tttt_1500_100.push_back(2.26291); m3_limit_T1tttt_1500_100.push_back(2.40375370178); m3_signif_T1tttt_1200_800.push_back(2.38843); m3_limit_T1tttt_1200_800.push_back(2.19742549629); 
    m3_signif_T1tttt_1500_100.push_back(2.29338); m3_limit_T1tttt_1500_100.push_back(2.57934716723); m3_signif_T1tttt_1200_800.push_back(2.42473); m3_limit_T1tttt_1200_800.push_back(2.38139463996); 
    m3_signif_T1tttt_1500_100.push_back(2.31948); m3_limit_T1tttt_1500_100.push_back(2.74530689786); m3_signif_T1tttt_1200_800.push_back(2.45512); m3_limit_T1tttt_1200_800.push_back(2.54726449266); 
    m3_signif_T1tttt_1500_100.push_back(2.34221); m3_limit_T1tttt_1500_100.push_back(2.90084617683); m3_signif_T1tttt_1200_800.push_back(2.48101); m3_limit_T1tttt_1200_800.push_back(2.68766629935); 
    m3_signif_T1tttt_1500_100.push_back(2.36225); m3_limit_T1tttt_1500_100.push_back(3.0567205057); m3_signif_T1tttt_1200_800.push_back(2.5034); m3_limit_T1tttt_1200_800.push_back(2.83656559983); 
    m3_signif_T1tttt_1500_100.push_back(2.3801); m3_limit_T1tttt_1500_100.push_back(3.19002670052); m3_signif_T1tttt_1200_800.push_back(2.52298); m3_limit_T1tttt_1200_800.push_back(2.96811649264); 
    m3_signif_T1tttt_1500_100.push_back(2.39614); m3_limit_T1tttt_1500_100.push_back(3.31391379847); m3_signif_T1tttt_1200_800.push_back(2.54028); m3_limit_T1tttt_1200_800.push_back(3.09365738363); 
    m3_signif_T1tttt_1500_100.push_back(2.41065); m3_limit_T1tttt_1500_100.push_back(3.44781219077); m3_signif_T1tttt_1200_800.push_back(2.55569); m3_limit_T1tttt_1200_800.push_back(3.2100358561); 
    m3_signif_T1tttt_1500_100.push_back(2.42385); m3_limit_T1tttt_1500_100.push_back(3.56794982035); m3_signif_T1tttt_1200_800.push_back(2.56952); m3_limit_T1tttt_1200_800.push_back(3.33550140925); 
    m3_signif_T1tttt_1500_100.push_back(2.43594); m3_limit_T1tttt_1500_100.push_back(3.67025005414); m3_signif_T1tttt_1200_800.push_back(2.58202); m3_limit_T1tttt_1200_800.push_back(3.42475136305); 
    m3_signif_T1tttt_1500_100.push_back(2.44705); m3_limit_T1tttt_1500_100.push_back(3.77860403253); m3_signif_T1tttt_1200_800.push_back(2.59338); m3_limit_T1tttt_1200_800.push_back(3.54324710251); 
    m3_signif_T1tttt_1500_100.push_back(2.4573); m3_limit_T1tttt_1500_100.push_back(3.8935351742); m3_signif_T1tttt_1200_800.push_back(2.60375); m3_limit_T1tttt_1200_800.push_back(3.64412894386); 
    m3_signif_T1tttt_1500_100.push_back(2.4668); m3_limit_T1tttt_1500_100.push_back(3.98442885205); m3_signif_T1tttt_1200_800.push_back(2.61326); m3_limit_T1tttt_1200_800.push_back(3.72363203068); 
    m3_signif_T1tttt_1500_100.push_back(2.47563); m3_limit_T1tttt_1500_100.push_back(4.07968439562); m3_signif_T1tttt_1200_800.push_back(2.62202); m3_limit_T1tttt_1200_800.push_back(3.80669597823);
    break;
  case kSystOnly:
    m1_signif_T1tttt_1500_100.push_back(2.52796); m1_limit_T1tttt_1500_100.push_back(0.59534795111); m1_signif_T1tttt_1200_800.push_back(1.66476); m1_limit_T1tttt_1200_800.push_back(0.270042558707); 
    m1_signif_T1tttt_1500_100.push_back(3.20748); m1_limit_T1tttt_1500_100.push_back(1.08016906806); m1_signif_T1tttt_1200_800.push_back(2.17773); m1_limit_T1tttt_1200_800.push_back(0.486691423037); 
    m1_signif_T1tttt_1500_100.push_back(3.59049); m1_limit_T1tttt_1500_100.push_back(1.52835791697); m1_signif_T1tttt_1200_800.push_back(2.48973); m1_limit_T1tttt_1200_800.push_back(0.682668414298); 
    m1_signif_T1tttt_1500_100.push_back(3.83824); m1_limit_T1tttt_1500_100.push_back(1.95419751856); m1_signif_T1tttt_1200_800.push_back(2.70593); m1_limit_T1tttt_1200_800.push_back(0.867799434195); 
    m1_signif_T1tttt_1500_100.push_back(4.01261); m1_limit_T1tttt_1500_100.push_back(2.35944770048); m1_signif_T1tttt_1200_800.push_back(2.86568); m1_limit_T1tttt_1200_800.push_back(1.04489823214); 
    m1_signif_T1tttt_1500_100.push_back(4.14301); m1_limit_T1tttt_1500_100.push_back(2.74530689786); m1_signif_T1tttt_1200_800.push_back(2.98884); m1_limit_T1tttt_1200_800.push_back(1.21326977417); 
    m1_signif_T1tttt_1500_100.push_back(4.24499); m1_limit_T1tttt_1500_100.push_back(3.11246261154); m1_signif_T1tttt_1200_800.push_back(3.08684); m1_limit_T1tttt_1200_800.push_back(1.38005462256); 
    m1_signif_T1tttt_1500_100.push_back(4.32746); m1_limit_T1tttt_1500_100.push_back(3.47118568761); m1_signif_T1tttt_1200_800.push_back(3.16683); m1_limit_T1tttt_1200_800.push_back(1.54682869182); 
    m1_signif_T1tttt_1500_100.push_back(4.3959); m1_limit_T1tttt_1500_100.push_back(3.83520875041); m1_signif_T1tttt_1200_800.push_back(3.23347); m1_limit_T1tttt_1200_800.push_back(1.70099559272); 
    m1_signif_T1tttt_1500_100.push_back(4.45386); m1_limit_T1tttt_1500_100.push_back(4.1795885613); m1_signif_T1tttt_1200_800.push_back(3.28997); m1_limit_T1tttt_1200_800.push_back(1.86181948171); 
    m1_signif_T1tttt_1500_100.push_back(4.50374); m1_limit_T1tttt_1500_100.push_back(4.51100685673); m1_signif_T1tttt_1200_800.push_back(3.33858); m1_limit_T1tttt_1200_800.push_back(2.02371387924); 
    m1_signif_T1tttt_1500_100.push_back(4.54726); m1_limit_T1tttt_1500_100.push_back(4.80750740356); m1_signif_T1tttt_1200_800.push_back(3.38092); m1_limit_T1tttt_1200_800.push_back(2.1603358026); 
    m1_signif_T1tttt_1500_100.push_back(4.58565); m1_limit_T1tttt_1500_100.push_back(5.12001310723); m1_signif_T1tttt_1200_800.push_back(3.41819); m1_limit_T1tttt_1200_800.push_back(2.30630497654); 
    m1_signif_T1tttt_1500_100.push_back(4.61983); m1_limit_T1tttt_1500_100.push_back(5.40368206896); m1_signif_T1tttt_1200_800.push_back(3.4513); m1_limit_T1tttt_1200_800.push_back(2.44976151572); 
    m1_signif_T1tttt_1500_100.push_back(4.65052); m1_limit_T1tttt_1500_100.push_back(5.70473435904); m1_signif_T1tttt_1200_800.push_back(3.48096); m1_limit_T1tttt_1200_800.push_back(2.57934716723); 
    m1_signif_T1tttt_1500_100.push_back(4.67825); m1_limit_T1tttt_1500_100.push_back(5.9708621925); m1_signif_T1tttt_1200_800.push_back(3.50771); m1_limit_T1tttt_1200_800.push_back(2.70184833445); 
    m1_signif_T1tttt_1500_100.push_back(4.70348); m1_limit_T1tttt_1500_100.push_back(6.2249058483); m1_signif_T1tttt_1200_800.push_back(3.53199); m1_limit_T1tttt_1200_800.push_back(2.83656559983); 
    m1_signif_T1tttt_1500_100.push_back(4.72653); m1_limit_T1tttt_1500_100.push_back(6.50157012919); m1_signif_T1tttt_1200_800.push_back(3.55413); m1_limit_T1tttt_1200_800.push_back(2.95101027837); 
    m1_signif_T1tttt_1500_100.push_back(4.74772); m1_limit_T1tttt_1500_100.push_back(6.75908590122); m1_signif_T1tttt_1200_800.push_back(3.57446); m1_limit_T1tttt_1200_800.push_back(3.07507803011); 
    m1_signif_T1tttt_1500_100.push_back(4.76726); m1_limit_T1tttt_1500_100.push_back(6.98978094027); m1_signif_T1tttt_1200_800.push_back(3.59317); m1_limit_T1tttt_1200_800.push_back(3.19002670052); 
    m2_signif_T1tttt_1500_100.push_back(2.06797); m2_limit_T1tttt_1500_100.push_back(0.66840898609); m2_signif_T1tttt_1200_800.push_back(1.75495); m2_limit_T1tttt_1200_800.push_back(0.549356977658); 
    m2_signif_T1tttt_1500_100.push_back(2.6476); m2_limit_T1tttt_1500_100.push_back(1.14798173331); m2_signif_T1tttt_1200_800.push_back(2.28709); m2_limit_T1tttt_1200_800.push_back(0.911028916058); 
    m2_signif_T1tttt_1500_100.push_back(2.98656); m2_limit_T1tttt_1500_100.push_back(1.55623130577); m2_signif_T1tttt_1200_800.push_back(2.60975); m2_limit_T1tttt_1200_800.push_back(1.21326977417); 
    m2_signif_T1tttt_1500_100.push_back(3.21734); m2_limit_T1tttt_1500_100.push_back(1.90334436639); m2_signif_T1tttt_1200_800.push_back(2.83192); m2_limit_T1tttt_1200_800.push_back(1.47550513918); 
    m2_signif_T1tttt_1500_100.push_back(3.38879); m2_limit_T1tttt_1500_100.push_back(2.22608571766); m2_signif_T1tttt_1200_800.push_back(2.99632); m2_limit_T1tttt_1200_800.push_back(1.70666521031); 
    m2_signif_T1tttt_1500_100.push_back(3.52365); m2_limit_T1tttt_1500_100.push_back(2.52216987319); m2_signif_T1tttt_1200_800.push_back(3.12411); m2_limit_T1tttt_1200_800.push_back(1.91760437521); 
    m2_signif_T1tttt_1500_100.push_back(3.634); m2_limit_T1tttt_1500_100.push_back(2.79019414171); m2_signif_T1tttt_1200_800.push_back(3.22703); m2_limit_T1tttt_1200_800.push_back(2.12448189198); 
    m2_signif_T1tttt_1500_100.push_back(3.72698); m2_limit_T1tttt_1500_100.push_back(3.03857162825); m2_signif_T1tttt_1200_800.push_back(3.31221); m2_limit_T1tttt_1200_800.push_back(2.31674006871); 
    m2_signif_T1tttt_1500_100.push_back(3.807); m2_limit_T1tttt_1500_100.push_back(3.27156616415); m2_signif_T1tttt_1200_800.push_back(3.38422); m2_limit_T1tttt_1200_800.push_back(2.47342918696); 
    m2_signif_T1tttt_1500_100.push_back(3.87706); m2_limit_T1tttt_1500_100.push_back(3.49487825592); m2_signif_T1tttt_1200_800.push_back(3.44615); m2_limit_T1tttt_1200_800.push_back(2.65285062063); 
    m2_signif_T1tttt_1500_100.push_back(3.93922); m2_limit_T1tttt_1500_100.push_back(3.69674834016); m2_signif_T1tttt_1200_800.push_back(3.50016); m2_limit_T1tttt_1200_800.push_back(2.81318483468); 
    m2_signif_T1tttt_1500_100.push_back(3.99498); m2_limit_T1tttt_1500_100.push_back(3.92336876135); m2_signif_T1tttt_1200_800.push_back(3.54781); m2_limit_T1tttt_1200_800.push_back(2.96811649264); 
    m2_signif_T1tttt_1500_100.push_back(4.04546); m2_limit_T1tttt_1500_100.push_back(4.11245085621); m2_signif_T1tttt_1200_800.push_back(3.59029); m2_limit_T1tttt_1200_800.push_back(3.11246261154); 
    m2_signif_T1tttt_1500_100.push_back(4.09147); m2_limit_T1tttt_1500_100.push_back(4.2845268597); m2_signif_T1tttt_1200_800.push_back(3.62844); m2_limit_T1tttt_1200_800.push_back(3.25079563223); 
    m2_signif_T1tttt_1500_100.push_back(4.1337); m2_limit_T1tttt_1500_100.push_back(4.4716119714); m2_signif_T1tttt_1200_800.push_back(3.66298); m2_limit_T1tttt_1200_800.push_back(3.39072703969); 
    m2_signif_T1tttt_1500_100.push_back(4.17264); m2_limit_T1tttt_1500_100.push_back(4.63349087202); m2_signif_T1tttt_1200_800.push_back(3.69442); m2_limit_T1tttt_1200_800.push_back(3.51889647407); 
    m2_signif_T1tttt_1500_100.push_back(4.20874); m2_limit_T1tttt_1500_100.push_back(4.80750740356); m2_signif_T1tttt_1200_800.push_back(3.72323); m2_limit_T1tttt_1200_800.push_back(3.64412894386); 
    m2_signif_T1tttt_1500_100.push_back(4.24235); m2_limit_T1tttt_1500_100.push_back(4.94687060965); m2_signif_T1tttt_1200_800.push_back(3.74974); m2_limit_T1tttt_1200_800.push_back(3.77860403253); 
    m2_signif_T1tttt_1500_100.push_back(4.27373); m2_limit_T1tttt_1500_100.push_back(5.09452898532); m2_signif_T1tttt_1200_800.push_back(3.77425); m2_limit_T1tttt_1200_800.push_back(3.8935351742); 
    m2_signif_T1tttt_1500_100.push_back(4.30313); m2_limit_T1tttt_1500_100.push_back(5.26479275143); m2_signif_T1tttt_1200_800.push_back(3.79699); m2_limit_T1tttt_1200_800.push_back(4.01569332953); 
    m3_signif_T1tttt_1500_100.push_back(2.11745); m3_limit_T1tttt_1500_100.push_back(0.697549508576); m3_signif_T1tttt_1200_800.push_back(1.95433); m3_limit_T1tttt_1200_800.push_back(0.615384615385); 
    m3_signif_T1tttt_1500_100.push_back(2.71477); m3_limit_T1tttt_1500_100.push_back(1.2018782954); m3_signif_T1tttt_1200_800.push_back(2.55821); m3_limit_T1tttt_1200_800.push_back(1.04918032787); 
    m3_signif_T1tttt_1500_100.push_back(3.06741); m3_limit_T1tttt_1500_100.push_back(1.61514097758); m3_signif_T1tttt_1200_800.push_back(2.9326); m3_limit_T1tttt_1200_800.push_back(1.41046906559); 
    m3_signif_T1tttt_1500_100.push_back(3.31023); m3_limit_T1tttt_1500_100.push_back(1.97683544229); m3_signif_T1tttt_1200_800.push_back(3.19675); m3_limit_T1tttt_1200_800.push_back(1.7355928438); 
    m3_signif_T1tttt_1500_100.push_back(3.49274); m3_limit_T1tttt_1500_100.push_back(2.29596346663); m3_signif_T1tttt_1200_800.push_back(3.39715); m3_limit_T1tttt_1200_800.push_back(2.02371387924); 
    m3_signif_T1tttt_1500_100.push_back(3.63792); m3_limit_T1tttt_1500_100.push_back(2.6055983887); m3_signif_T1tttt_1200_800.push_back(3.55666); m3_limit_T1tttt_1200_800.push_back(2.27555620282); 
    m3_signif_T1tttt_1500_100.push_back(3.75798); m3_limit_T1tttt_1500_100.push_back(2.87640656281); m3_signif_T1tttt_1200_800.push_back(3.68805); m3_limit_T1tttt_1200_800.push_back(2.52216987319); 
    m3_signif_T1tttt_1500_100.push_back(3.8601); m3_limit_T1tttt_1500_100.push_back(3.13149785806); m3_signif_T1tttt_1200_800.push_back(3.79907); m3_limit_T1tttt_1200_800.push_back(2.75269006637); 
    m3_signif_T1tttt_1500_100.push_back(3.94878); m3_limit_T1tttt_1500_100.push_back(3.37954295061); m3_signif_T1tttt_1200_800.push_back(3.89471); m3_limit_T1tttt_1200_800.push_back(2.96811649264); 
    m3_signif_T1tttt_1500_100.push_back(4.02704); m3_limit_T1tttt_1500_100.push_back(3.59298649037); m3_signif_T1tttt_1200_800.push_back(3.97839); m3_limit_T1tttt_1200_800.push_back(3.17027549694); 
    m3_signif_T1tttt_1500_100.push_back(4.097); m3_limit_T1tttt_1500_100.push_back(3.80669597823); m3_signif_T1tttt_1200_800.push_back(4.05252); m3_limit_T1tttt_1200_800.push_back(3.3573721177); 
    m3_signif_T1tttt_1500_100.push_back(4.16016); m3_limit_T1tttt_1500_100.push_back(4.01569332953); m3_signif_T1tttt_1200_800.push_back(4.11886); m3_limit_T1tttt_1200_800.push_back(3.54324710251); 
    m3_signif_T1tttt_1500_100.push_back(4.2177); m3_limit_T1tttt_1500_100.push_back(4.21398622027); m3_signif_T1tttt_1200_800.push_back(4.17875); m3_limit_T1tttt_1200_800.push_back(3.72363203068); 
    m3_signif_T1tttt_1500_100.push_back(4.27044); m3_limit_T1tttt_1500_100.push_back(4.39485099258); m3_signif_T1tttt_1200_800.push_back(4.23319); m3_limit_T1tttt_1200_800.push_back(3.8935351742); 
    m3_signif_T1tttt_1500_100.push_back(4.31908); m3_limit_T1tttt_1500_100.push_back(4.57142857143); m3_signif_T1tttt_1200_800.push_back(4.28299); m3_limit_T1tttt_1200_800.push_back(4.04743594933); 
    m3_signif_T1tttt_1500_100.push_back(4.36417); m3_limit_T1tttt_1500_100.push_back(4.76278927991); m3_signif_T1tttt_1200_800.push_back(4.32879); m3_limit_T1tttt_1200_800.push_back(4.1795885613); 
    m3_signif_T1tttt_1500_100.push_back(4.40614); m3_limit_T1tttt_1500_100.push_back(4.8995110288); m3_signif_T1tttt_1200_800.push_back(4.37111); m3_limit_T1tttt_1200_800.push_back(4.35745036864); 
    m3_signif_T1tttt_1500_100.push_back(4.44539); m3_limit_T1tttt_1500_100.push_back(5.04433974637); m3_signif_T1tttt_1200_800.push_back(4.41038); m3_limit_T1tttt_1200_800.push_back(4.4716119714); 
    m3_signif_T1tttt_1500_100.push_back(4.48216); m3_limit_T1tttt_1500_100.push_back(5.21118319915); m3_signif_T1tttt_1200_800.push_back(4.44695); m3_limit_T1tttt_1200_800.push_back(4.63349087202); 
    m3_signif_T1tttt_1500_100.push_back(4.51674); m3_limit_T1tttt_1500_100.push_back(5.3753325987); m3_signif_T1tttt_1200_800.push_back(4.48111); m3_limit_T1tttt_1200_800.push_back(4.76278927991);
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
      ncz.at(i) = 4./ncz.at(i);
    }
    if(cz.at(i) <= 0.){
      cz.at(i) = big_num;
    }else{
      cz.at(i) = 4./cz.at(i);
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
  TH1D h("h", ";Luminosity [fb^{-1}];Expected Significance", 1, 0., 20.);
  h.SetMaximum(8.);
  TAxis &yaxis = *h.GetYaxis();
  yaxis.SetLabelColor(kRed);
  yaxis.SetTitleColor(kRed);
  yaxis.SetTitleOffset(.8);
  TCanvas c;
  TGaxis *raxis = new TGaxis(20., 0.,
                             20., 8.,
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
