#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include "util.h"
#include "equations.h"

template <int dim>
class Parameters
{
public:
  // Parameters constructor takes a triangulation as an attribute (passed by reference), and the constructor is responsible for filling out the triangulation.
  Parameters();

  void delete_old_outputs(MPI_Comm& mpi_communicator) const;

  bool is_periodic_boundary(int boundary_id) const;

  // Use exactly Div-Free space.
  bool use_div_free_space_for_B;

  // Gravity acceleration - in z-direction
  double g;

  // Limiter
  bool limit;
  enum Limiter { vertexBased, barthJespersen};
  Limiter slope_limiter;
  double start_limiting_at;
  bool limit_edges_and_vertices, limitB;

  // Flux enumeration - for potentially adding more fluxes, decision which one to use is then made in Equations<>::numerical_normal_flux.
  enum NumFluxType { hlld, lax_friedrich };
  NumFluxType num_flux_type;
  // A special value for lax_friedrich
  double lax_friedrich_stabilization_value;

  // Output step - either < 0 (output all steps), or > 0 (time difference between two outputs)
  double output_step;
  // File name
  std::string output_file_prefix;

  // Output matrix after assemble_system() in Problem::run().
  bool output_matrix;
  // Output rhs after assemble_system() in Problem::run().
  bool output_rhs;
  // Output limited_solution after solve() in Problem::run().
  bool output_solution;

  // Number of patches
  unsigned int patches; 
  
  // Gas gamma value.
  double gas_gamma;

  // Linear solver type enumeration
  enum SolverType { gmres, direct };
  // Linear solver type selected
  SolverType solver;
  // Verbosity enumeration
  enum  OutputType { quiet_solver, verbose_solver };
  // Verbosity selected
  OutputType output;

  // Tolerance for linear residual norm
  double linear_residual;
  // Maximum allowed linear iterations count, succeed the linear loop exceeded
  int max_iterations;
  // Linear solver parameters.
  double ilut_fill;
  double ilut_atol;
  double ilut_rtol;
  double ilut_drop;

  // Global - obvious
  double current_time_step_length, final_time, cfl_coefficient;
  // Polynomial order for the flow part.
  int polynomial_order_dg;
  // Quadrature order.
  int quadrature_order;

  Point<dim> corner_a;
  Point<dim> corner_b;
  std::vector<unsigned int> refinements;
  std::vector<std::array<int, 3> > periodic_boundaries;

  // Debugging
  enum DebuggingFlag
  {
    None = 0,
    BasicSteps = 1,
    PeriodicBoundaries = 2,
    Assembling = 4,
    SlopeLimiting = 8,
    NumFlux = 16,
    Adaptivity = 32,
    DetailSteps = 64
  };
  int debug;

  // Adaptivity
  int max_cells;
  int refine_every_nth_time_step;
  int perform_n_initial_refinements;
  double refine_threshold;
  double coarsen_threshold;
  int volume_factor;
  double time_interval_max_cells_multiplicator;
};

#endif