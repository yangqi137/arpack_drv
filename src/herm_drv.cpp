#include "herm_drv.hpp"
#include "bindings/arpack_interface.h"
#include "fortinfo_error.hpp"
#include <stdexcept>
#include <limits>
#include <iostream>

arpack::HermDrv::HermDrv(std::size_t n_, unsigned nev_, unsigned ncv_)
  : n(n_), nev(nev_), ncv(ncv_),
    resid(n), v(n, ncv), workd(3*n),
    lworkl(3*ncv*ncv+5*ncv), workl(lworkl), rwork(ncv),
    select(new FORTRAN_LOGICAL[ncv]), d(nev+1), d_real(nev),
    workev(2*ncv),
    maxiter(iparam[2]), nconv(iparam[4]) {
  if (n_>std::numeric_limits<FORTRAN_INT>::max())
    throw std::invalid_argument("arpack::HermDrv: "
				"dimension exceeds upper limit of"
				"fortran integer type");

  maxiter=500;
  iparam[0]=1; //ishift
  iparam[3]=1; //NB must be 1
  iparam[6]=1; // Mode
}

void arpack::HermDrv::iterate(FORTRAN_INT& ido) {
  double tol=0;
  FORTRAN_INT ldv=n;
  FORTRAN_INT info=0;
  ARPACK_ZNAUPD(&ido, "I", &n, "SR", &nev, &tol, (dcomplex_t*)&resid[0],
		&ncv, (dcomplex_t*)&v(0,0), &ldv, iparam, ipntr,
		(dcomplex_t*)&workd[0], (dcomplex_t*)&workl[0], 
		&lworkl, &rwork[0],
		&info);
  if (info==0)
    return;
  else if (info<0)
    throw arpack::fortinfo_error("dsaupd", info);
  else
    std::cout<<"Warning: dsaupd info="<<info<<std::endl;

}

void arpack::HermDrv::post_process() {
  FORTRAN_LOGICAL rvec=1;
  FORTRAN_INT ldv=n;
  complex_d sigma; //will not be referenced
  double tol=0;
  FORTRAN_INT info=0;

  ARPACK_ZNEUPD(&rvec, "A", select.get(), (dcomplex_t*)&d[0], 
		(dcomplex_t*)&v(0,0), &ldv,
		(const dcomplex_t*)&sigma, (dcomplex_t*)&workev[0],
		"I", &n, "SR", &nev, &tol, (dcomplex_t*)&resid[0],
		&ncv, (dcomplex_t*)&v(0,0), &ldv, iparam, ipntr,
		(dcomplex_t*)&workd[0], (dcomplex_t*)&workl[0],
		&lworkl, &rwork[0],
		&info);
  if (info!=0)
    throw arpack::fortinfo_error("dseupd", info);

  d_real=real(d);
}
