#ifndef DIMER_ARPACK_DRV_HPP
#define DIMER_ARPACK_DRV_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/blas.hpp>
#include <boost/numeric/bindings/ublas.hpp>
#include <boost/scoped_array.hpp>
#include <boost/ref.hpp>
#include <cmath>
#include <cassert>

namespace arpack {
  class BasicDrv {
  private:
    typedef boost::numeric::ublas::matrix<double, 
	boost::numeric::ublas::column_major> DMatrix;
    typedef boost::numeric::ublas::vector<double> DVector;
  public:
    BasicDrv(int n_, int nev_, int ncv_);

  public:
    template <typename func_type>
    void solve(const func_type& av_func) {
      using boost::numeric::ublas::subrange;
      using boost::numeric::ublas::column;
      using boost::numeric::bindings::blas::axpy;
      using boost::numeric::bindings::blas::nrm2;
      int ido=0;
      while (true) {
	iterate(ido);
	if (ido==-1 || ido==1)
	  av_func(subrange(workd, ipntr[0]-1, ipntr[0]+n-1), 
		  subrange(workd, ipntr[1]-1, ipntr[1]+n-1));
	else
	  break;
      }
      post_process();
      //int nconv=iparam[4];
      for (int j=0;j<nconv;j++) {
	av_func(column(v, j), &ax[0]);
	axpy(-d[j], column(v, j), ax);
	dres[j]=nrm2(ax);
	dres[j]/=std::abs(d[j]);
      }
    }

    void set_maxiter(int new_maxiter) {maxiter=new_maxiter;}
    
    int get_nconv() const {return iparam[4];}
    typedef boost::numeric::ublas::vector_range<const DVector>
    evals_return_type;
    evals_return_type evals() const {
      return boost::numeric::ublas::subrange(d, 0, nconv);
    }
    typedef boost::numeric::ublas::vector_range<const DVector> 
    evals_res_return_type;
    evals_res_return_type evals_res() const {
      return boost::numeric::ublas::subrange(dres, 0, nconv);
    }
    
    typedef boost::numeric::ublas::matrix_column<const DMatrix> 
    evec_return_type;
    evec_return_type evec(int i) const {return evec_return_type(v, i);}
    
  private:
    void iterate(int& ido);
    void post_process();
    
  private:
    int n;
    int nev;
    int ncv;
    
    DVector resid;
    DMatrix v;
    DVector workd;
    int lworkl;
    DVector workl;

    boost::scoped_array<int> select;
    DVector d;
    DVector ax;
    DVector dres;
    
    int iparam[11];
    int ipntr[11];
    
    int& maxiter;
    int& nconv;
  };
}
#endif
