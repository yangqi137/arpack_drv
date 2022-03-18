#include "../herm_drv.hpp"

#include <iostream>

static const unsigned N = 100;

struct Av {
  template <typename x_type, typename y_type>
  void operator() (x_type x, y_type y) const {
    for (unsigned i=0;i<N;i++) {
      y[i] = -5.*x[i];
      if (i>0)
	y[i] -= x[i-1];
      if (i<N-1)
	y[i] -= x[i+1];
    }
  }
};

int main(void) {
  arpack::HermDrv drv(N, 1, 20);
  Av av;
  drv.solve(av);
  std::cout<<"# conv: "<<drv.get_nconv()<<std::endl;
  std::cout<<"E0: "<<drv.evals()(0)<<std::endl;
}
