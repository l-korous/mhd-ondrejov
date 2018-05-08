#ifndef _UTIL_H
#define _UTIL_H

// This file is only for having all deal.II (and other) header files in one place.

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <memory>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <deal.II/base/quadrature_lib.h>
#include <deal.II/base/numbers.h>
#include <deal.II/base/function.h>
#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/function_parser.h>
#include <deal.II/base/utilities.h>
#include <deal.II/base/conditional_ostream.h>
#include <deal.II/base/std_cxx11/array.h>

#include <deal.II/lac/vector.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>

#include <deal.II/grid/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/grid_refinement.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_in.h>

#include <deal.II/dofs/dof_handler.h>
#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_tools.h>

#include <deal.II/fe/fe_values.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/fe/mapping_q1.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/fe_dgt.h>
// #include <deal.II/fe/fe_div_free.h> 
#include <deal.II/fe/fe_raviart_thomas.h>

#include <deal.II/numerics/data_out.h>
#include <deal.II/numerics/data_postprocessor.h>
#include <deal.II/numerics/vector_tools.h>
#include <deal.II/numerics/solution_transfer.h>

#include <deal.II/lac/trilinos_sparse_matrix.h>
#include <deal.II/lac/trilinos_precondition.h>
#include <deal.II/lac/trilinos_solver.h>
#include <Sacado.hpp>
#include <deal.II/lac/generic_linear_algebra.h>
#include <deal.II/base/index_set.h>
#include <deal.II/distributed/tria.h>
#include <deal.II/lac/sparsity_tools.h>
#include <deal.II/distributed/solution_transfer.h>
#include <deal.II/numerics/error_estimator.h>
#include <deal.II/numerics/derivative_approximation.h>

#ifndef NEGLIGIBLE
#define NEGLIGIBLE 1e-12
#endif
#ifndef SMALL
#define SMALL 1e-8
#endif

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
  std::ostringstream out;
  out << std::setprecision(n) << a_value;
  return out.str();
}

// For debug purposes only.
#ifndef OUTPUT_BASE
//#define OUTPUT_BASE
#endif

// Safe limit on max number of basis functions (for all components !!)
// - e.g. for linear Taylor functions in 3d, it is 4 x component count = 32 in case of MHD (8 components)
#define BASIS_FN_COUNT 32

static const double My_PI = 3.14159265358979323846;

using namespace dealii;
#endif