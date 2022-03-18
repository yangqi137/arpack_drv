#include "fortinfo_error.hpp"
#include <boost/format.hpp>

arpack::fortinfo_error::fortinfo_error(const char *func_name, int info)
  : std::runtime_error((boost::format("%1%: info=%2%") %func_name %info)
		       .str()) {}
