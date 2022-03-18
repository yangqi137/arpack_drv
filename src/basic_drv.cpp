#include "basic_drv.hpp"
#include "fortinfo_error.hpp"
#include <iostream>

extern "C" {
  //c\Usage:
  //c  call dsaupd  
  //c     ( IDO, BMAT, N, WHICH, NEV, TOL, RESID, NCV, V, LDV, IPARAM,
  //c       IPNTR, WORKD, WORKL, LWORKL, INFO )
  void dsaupd_(int *ido, const char *bmat, const int *n, const char *which,
	       const int *nev, const double *tol, double *resid,
	       const int *ncv, double *v, const int *ldv, int *iparam,
	       int *ipntr, double *workd, double *workl, const int *lworkl,
	       int *info);
  //c  call dseupd  
  //c     ( RVEC, HOWMNY, SELECT, D, Z, LDZ, SIGMA, BMAT, N, WHICH, NEV, TOL,
  //c       RESID, NCV, V, LDV, IPARAM, IPNTR, WORKD, WORKL, LWORKL, INFO )
  void dseupd_(const int *rvec, const char *howmny, int *select,
	       double *d, double *z, const int *ldz, const double *sigma,
	       const char *bmat, const int *n, const char *which,
	       const int *nev, const double *tol, double *resid,
	       const int *ncv, double *v, const int *ldv, int *iparam,
	       int *ipntr, double *workd, double *workl, const int *lworkl,
	       int *info);
}

#define ARPACK_DSAUPD dsaupd_
#define ARPACK_DSEUPD dseupd_

arpack::BasicDrv::BasicDrv(int n_, int nev_, int ncv_)
  : n(n_), nev(nev_), ncv(ncv_),
    resid(n), v(n, ncv), workd(3*n), 
    lworkl(ncv*ncv+8*ncv), workl(lworkl),
    select(new int[ncv]), d(nev), ax(n), dres(nev),
    maxiter(iparam[2]), nconv(iparam[4]) {
  maxiter=500;
  iparam[0]=1; //ishift
  // iparam[1] no longer referenced.
  iparam[3]=1; // NB, must be 1
  // iparam[4]=nconv output parameter.
  // iparam[5] no longer referenced.
  iparam[6] = 1; // Mode
  // iparam[7-10] all output parameters.
}

void arpack::BasicDrv::iterate(int &ido) {
  double tol=0;
  int ldv=n;
  int info=0;
  //std::cout<<maxiter<<std::endl;
  ARPACK_DSAUPD(&ido, "I", &n, "SA", &nev, &tol, &resid[0], 
                &ncv, &v(0,0), &ldv, iparam, ipntr, &workd[0], 
                &workl[0], &lworkl, &info);
  if (info==0)
    return;
  else if (info<0)
    throw arpack::fortinfo_error("dsaupd", info);
  else
    std::cout<<"Warning: dsaupd info="<<info<<std::endl;
}

void arpack::BasicDrv::post_process() {
  int rvec = 1;
  int ldv=n;
  double sigma; // will not be referenced.
  double tol=0;
  int info;
  //std::cout<<maxiter<<std::endl;
  ARPACK_DSEUPD(&rvec, "A", select.get(), &d[0], &v(0,0), &ldv, &sigma,
		"I", &n, "SA", &nev, &tol, &resid[0],
		&ncv, &v(0,0), &ldv, iparam, ipntr, &workd[0], 
		&workl[0], &lworkl, &info);

  if (info!=0)
    throw arpack::fortinfo_error("dseupd", info);
}
