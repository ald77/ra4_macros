#include "gamma_params.hpp"

#include <cmath>

#include "utilities.hpp"

GammaParams::GammaParams():
  n_effective_(0.),
  weight_(0.){
}

GammaParams::GammaParams(double n_effective, double weight):
  n_effective_(n_effective),
  weight_(weight){
}

void GammaParams::SetYieldAndUncertainty(double yield, double uncertainty){
  if(yield > 0.){
    n_effective_ = (yield*yield)/(uncertainty*uncertainty);
    weight_ = uncertainty*uncertainty/yield;
  }else{
    n_effective_ = 0.;
    weight_ = uncertainty;
  }
}

void GammaParams::SetNEffectiveAndWeight(double n_effective, double weight){
  n_effective_ = n_effective;
  weight_ = weight;
}

double GammaParams::Yield() const{
  return n_effective_*weight_;
}

void GammaParams::Yield(double yield){
  SetYieldAndUncertainty(yield, Uncertainty());
}

double GammaParams::Uncertainty() const{
  return sqrt(n_effective_)*weight_;
}

void GammaParams::Uncertainty(double uncertainty){
  SetYieldAndUncertainty(Yield(), uncertainty);
}

double GammaParams::NEffective() const{
  return n_effective_;
}

void GammaParams::NEffective(double n_effective){
  SetNEffectiveAndWeight(n_effective, Weight());
}

double GammaParams::Weight() const{
  return weight_;
}

void GammaParams::Weight(double weight){
  SetNEffectiveAndWeight(NEffective(), weight);
}

GammaParams & GammaParams::operator+=(const GammaParams &gp){
  if(NEffective() == 0. && gp.NEffective() == 0.){
    SetNEffectiveAndWeight(0., AddInQuadrature(Weight(), gp.Weight()));
  }else{
    SetYieldAndUncertainty(Yield()+gp.Yield(), AddInQuadrature(Uncertainty(),gp.Uncertainty()));
  }
  return *this;
}

GammaParams operator+(GammaParams gp1, GammaParams gp2){
  return (gp1 += gp2);
}
