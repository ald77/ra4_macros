#ifndef H_GAMMA_PARAM
#define H_GAMMA_PARAM

class GammaParams{
public:
  GammaParams();
  GammaParams(double n_effective, double weight);

  void SetNEffectiveAndWeight(double n_effective, double weight);
  void SetYieldAndUncertainty(double yield, double uncertainty);

  double Yield() const;
  void Yield(double yield);

  double Uncertainty() const;
  void Uncertainty(double uncertainty);

  double NEffective() const;
  void NEffective(double n_effective);

  double Weight() const;
  void Weight(double weight);

  GammaParams & operator+=(const GammaParams &gp);

private:
  double n_effective_, weight_;
};

GammaParams operator+(GammaParams gp1, GammaParams gp2);

#endif
