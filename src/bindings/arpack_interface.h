#ifndef ARPACK_DRV_BINDINGS_ARPACK_INTERFACE_H
#define ARPACK_DRV_BINDINGS_ARPACK_INTERFACE_H

#include <boost/numeric/bindings/traits/type.h>
#include "arpack_names.h"
#include "fortran_types.h"

extern "C" {
  //c\Usage:
  //c  call dsaupd  
  //c     ( IDO, BMAT, N, WHICH, NEV, TOL, RESID, NCV, V, LDV, IPARAM,
  //c       IPNTR, WORKD, WORKL, LWORKL, INFO )
  void ARPACK_DSAUPD
  (int *ido, const char *bmat, const int *n, const char *which,
   const int *nev, const double *tol, double *resid,
   const int *ncv, double *v, const int *ldv, int *iparam,
   int *ipntr, double *workd, double *workl, const int *lworkl,
   int *info);
  //c  call dseupd  
  //c     ( RVEC, HOWMNY, SELECT, D, Z, LDZ, SIGMA, BMAT, N, WHICH, NEV, TOL,
  //c       RESID, NCV, V, LDV, IPARAM, IPNTR, WORKD, WORKL, LWORKL, INFO )
  void ARPACK_DSEUPD
  (const int *rvec, const char *howmny, int *select,
   double *d, double *z, const int *ldz, const double *sigma,
   const char *bmat, const int *n, const char *which,
   const int *nev, const double *tol, double *resid,
   const int *ncv, double *v, const int *ldv, int *iparam,
   int *ipntr, double *workd, double *workl, const int *lworkl,
   int *info);

  //c\Usage:
  //c  call znaupd
  //c     ( IDO, BMAT, N, WHICH, NEV, TOL, RESID, NCV, V, LDV, IPARAM,
  //c       IPNTR, WORKD, WORKL, LWORKL, RWORK, INFO )
  void ARPACK_ZNAUPD
  (FORTRAN_INT *ido, const char *bmat, const FORTRAN_INT *n,
   const char *which, const FORTRAN_INT *nev, const double *tol, 
   dcomplex_t *resid, const FORTRAN_INT *ncv, dcomplex_t *v, 
   const FORTRAN_INT *ldv, FORTRAN_INT *iparam,
   FORTRAN_INT *ipntr, dcomplex_t *workd, dcomplex_t *workl, 
   const FORTRAN_INT *lworkl, double *rwork, FORTRAN_INT *info);

  //c\Usage:
  //c  call zneupd 
  //c     ( RVEC, HOWMNY, SELECT, D, Z, LDZ, SIGMA, WORKEV, BMAT, 
  //c       N, WHICH, NEV, TOL, RESID, NCV, V, LDV, IPARAM, IPNTR, WORKD, 
  //c       WORKL, LWORKL, RWORK, INFO )
  void ARPACK_ZNEUPD
  (const FORTRAN_LOGICAL *rvec, const char *howmny, const int *select,
   dcomplex_t *d, dcomplex_t *z, const FORTRAN_INT *ldz, 
   const dcomplex_t *sigma, dcomplex_t *workev,
   const char *bmat, const FORTRAN_INT *n,
   const char *which, const FORTRAN_INT *nev, const double *tol, 
   dcomplex_t *resid, const FORTRAN_INT *ncv, dcomplex_t *v, 
   const FORTRAN_INT *ldv, FORTRAN_INT *iparam,
   FORTRAN_INT *ipntr, dcomplex_t *workd, dcomplex_t *workl, 
   const FORTRAN_INT *lworkl, double *rwork, FORTRAN_INT *info);

}

#endif
