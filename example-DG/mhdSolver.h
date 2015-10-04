#ifndef MHD_SOLVER_H
#define MHD_SOLVER_H

#include <deal.II/fe/fe_system.h>
#include <deal.II/meshworker/simple.h>
#include <deal.II/meshworker/loop.h>


#include "definitions.h"

class MHDSolver
{
public:
  MHDSolver();
  void run();

private:
  void setup_system();
  void assemble_system(bool firstIteration);
  void solve(dealii::Vector<d> &solution);
  void solveOneStep(dealii::Vector<d> &solution);
  void outputResults(ui timeStep, d currentTime) const;
  void add_markers(dealii::Triangulation<DIM>::cell_iterator cell);

  dealii::Triangulation<DIM>   triangulation;
  dealii::FESystem<DIM> feSystem;
  const dealii::MappingQ1<DIM> mapping;
  const dealii::QGauss<DIM> quad;
  const dealii::QGauss<DIM - 1> quadFace;
  dealii::DoFHandler<DIM>      dofHandler;
  dealii::ConstraintMatrix     hangingNodeConstraints;

  dealii::SparsityPattern      sparsityPattern;
  dealii::SparseMatrix<d> systemMatrix;

  dealii::Vector<d>       solution;
  static dealii::Vector<d>       slnPrev;
  static dealii::Vector<d> slnUtil;

  dealii::Vector<d>       rightHandSide;

  typedef dealii::MeshWorker::DoFInfo<DIM> DoFInfo;
  typedef dealii::MeshWorker::IntegrationInfo<DIM> CellInfo;

  static void assembleVolumetric(DoFInfo &dinfo, CellInfo &info);
  static void assembleBoundaryEdge(DoFInfo &dinfo, CellInfo &info);
  static void assembleInternalEdge(DoFInfo &dinfo1, DoFInfo &dinfo2, CellInfo &info1, CellInfo &info2);
};

#endif
