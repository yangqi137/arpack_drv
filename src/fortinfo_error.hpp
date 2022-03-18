#ifndef DIMER_FORTRAN_ERROR_WITH_INFO_HPP
#define DIMER_FORTRAN_ERROR_WITH_INFO_HPP

#include <stdexcept>
namespace arpack {
  class fortinfo_error : public std::runtime_error {
  public:
    fortinfo_error(const char *func_name, int info);
    
    const char *func_name;
    int info;
  };
}

#endif
