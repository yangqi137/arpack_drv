#ifndef ARPACK_DRV_HERMITION_DRV_HPP
#define ARPACK_DRV_HERMITION_DRV_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/bindings/blas.hpp>
#include <boost/numeric/bindings/ublas.hpp>

//#include <boost/numeric/bindings/traits/type_traits.hpp>
#include <boost/scoped_array.hpp>

#include "bindings/fortran_types.h"

namespace arpack {
  class HermDrv {
  public:
    typedef std::complex<double> complex_d;

  private:
    typedef boost::numeric::ublas::matrix<complex_d,
	      boost::numeric::ublas::column_major> matrix_t;
    typedef boost::numeric::ublas::vector<double> dvector_t;
    typedef boost::numeric::ublas::vector<complex_d> zvector_t;

  public:
    HermDrv(std::size_t n_, unsigned nev_, unsigned ncv_);

  public:
    template <typename func_type>
    void solve(const func_type& av_func) {
      using boost::numeric::ublas::subrange;
      using boost::numeric::ublas::column;

      FORTRAN_INT ido=0;
      while (true) {
	iterate(ido);
	if (ido==-1 || ido==1)
	  av_func(subrange(workd, ipntr[0]-1, ipntr[0]+n-1), 
		  subrange(workd, ipntr[1]-1, ipntr[1]+n-1));
	else
	  break;
      }
      post_process();
    }

    void set_maxiter(unsigned new_maxiter) {maxiter = new_maxiter;}
    unsigned get_maxiter() {return maxiter;}
    
    unsigned get_nconv() const {return nconv;}
    typedef boost::numeric::ublas::vector_range<const dvector_t>
    evals_return_type;
    evals_return_type evals() const {
      return boost::numeric::ublas::subrange(d_real, 0, nconv);
    }

    typedef boost::numeric::ublas::matrix_column<const matrix_t>
    evec_return_type;
    evec_return_type evec(unsigned i) const {return evec_return_type(v, i);}

  private:
    void iterate(FORTRAN_INT& ido);
    void post_process();

  private:
    FORTRAN_INT n;
    FORTRAN_INT nev;
    FORTRAN_INT ncv;

    zvector_t resid;
    matrix_t v;
    zvector_t workd;
    FORTRAN_INT lworkl;
    zvector_t workl;
    dvector_t rwork;
    boost::scoped_array<FORTRAN_LOGICAL> select;
    zvector_t d;
    dvector_t d_real;
    zvector_t workev;
    
    FORTRAN_INT iparam[11];
    FORTRAN_INT ipntr[14];

    FORTRAN_INT& maxiter;
    FORTRAN_INT& nconv;
  };
}

#endif
