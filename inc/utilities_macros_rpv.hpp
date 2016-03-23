namespace rpv
{
  TString luminosity="2.69";
}

TString filestring(TString dataset, bool isSkimmed=true)
{
  if(isSkimmed) {
    //    TString datasetString(Form("/mnt/hadoop/local/cawest/skims/ht1000/*%s*.root", dataset.Data()));
    TString datasetString(Form("/net/cms29/cms29r0/cawest/skims/ht1200/*%s*.root", dataset.Data()));
    return datasetString;
  }
  else {
    TString datasetString(Form("/homes/cawest/links/%s/*.root", dataset.Data()));
    return datasetString;
  }

}

// this is a hack to add an additional weight
std::string cutandweight(std::string cut, std::string weight)
{
  std::string newcut("(");
  newcut+=cut;
  newcut+="))*(";
  newcut+=weight;

  return newcut;
} 
