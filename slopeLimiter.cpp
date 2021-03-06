#include "slopeLimiter.h"
#include "problem.h"

template <EquationsType equationsType, int dim>
void VertexBasedSlopeLimiter<equationsType, dim>::flush_cache()
{
  this->postprocessData.clear();
}

template <EquationsType equationsType, int dim>
void VertexBasedSlopeLimiter<equationsType, dim>::postprocess(TrilinosWrappers::MPI::Vector& current_limited_solution, TrilinosWrappers::MPI::Vector& current_unlimited_solution)
{
  int cell_count = 0;
  // Loop through all cells.
  for (typename DoFHandler<dim>::active_cell_iterator cell = this->dof_handler.begin_active(); cell != this->dof_handler.end(); ++cell)
  {
    if (!cell->is_locally_owned())
      continue;

    bool u_c_set[Equations<equationsType, dim>::n_components];
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      u_c_set[i] = false;

    double u_c[Equations<equationsType, dim>::n_components];
    cell->get_dof_indices(this->dof_indices);

    typename SlopeLimiter<equationsType, dim>::PostprocessData* data = 0;
    auto it = this->postprocessData.find(cell->active_cell_index());
    if (it != this->postprocessData.end())
      data = &(it->second);
    else
    {
      data = &(((this->postprocessData.insert(std::make_pair(cell->active_cell_index(), typename SlopeLimiter<equationsType, dim>::PostprocessData()))).first)->second);
      for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
        u_c_set[i] = false;
      for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
      {
        if (!this->is_primitive[i])
          data->lambda_indices_to_multiply_all_B_components.push_back(this->dof_indices[i]);
        else
        {
          // Here we rely on the fact, that the constant basis fn is the first one and all other basis fns come after.
          if (!u_c_set[this->component_ii[i]])
            u_c_set[this->component_ii[i]] = true;
          else
            data->lambda_indices_to_multiply[this->component_ii[i]].push_back(this->dof_indices[i]);
        }
      }
      for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
        data->vertex_is_at_nonperiodic_boundary[vertex_i] = false;

      for (unsigned int face = 0; face<GeometryInfo<dim>::faces_per_cell; ++face)
        if (cell->at_boundary(face) && !(this->parameters.is_periodic_boundary(cell->face(face)->boundary_id())))
          for (unsigned int v = 0; v < GeometryInfo<dim>::vertices_per_face; ++v)
          {
            int index_ = GeometryInfo<dim>::face_to_cell_vertices(face, v);
            data->vertex_is_at_nonperiodic_boundary[index_] = true;
          }

      data->center = cell->center();
      for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
      {
        data->vertexIndex[vertex_i] = cell->vertex_index(vertex_i);
        data->vertexPoint[vertex_i] = data->center + (1. - NEGLIGIBLE) * (cell->vertex(vertex_i) - data->center);

        unsigned short neighbor_i = 0;

        for (auto neighbor_element : GridTools::find_cells_adjacent_to_vertex(this->triangulation, data->vertexIndex[vertex_i]))
        {
          typename DoFHandler<dim>::active_cell_iterator neighbor(&this->triangulation, neighbor_element->level(), neighbor_element->index(), &this->dof_handler);
          if (neighbor->active_cell_index() != cell->active_cell_index())
          {
            data->neighbor_dof_indices[vertex_i][neighbor_i].resize(this->dofs_per_cell);
            neighbor->get_dof_indices(data->neighbor_dof_indices[vertex_i][neighbor_i++]);
          }
        }
        data->neighbor_count = neighbor_i;
      }
    }

    // Cell center value we must find in any case (new data or reused)
    // - let us reuse this array for that.
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      u_c_set[i] = false;
    for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
    {
      if (this->is_primitive[i])
      {
        // Here we rely on the fact, that the constant basis fn is the first one and that all other basis fns have zero mean.
        if (!u_c_set[this->component_ii[i]])
        {
          u_c[this->component_ii[i]] = current_unlimited_solution(this->dof_indices[i]);
          u_c_set[this->component_ii[i]] = true;
        }
      }
    }

    if (this->parameters.debug & this->parameters.SlopeLimiting)
      LOGL(2, "cell: " << ++cell_count << " - center: " << data->center << ", values: " << u_c[0] << ", " << u_c[1] << ", " << u_c[2] << ", " << u_c[3] << ", " << u_c[4]);

    double alpha_e[Equations<equationsType, dim>::n_components];
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      alpha_e[i] = 1.;

    for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
    {
      if (!this->parameters.limit_edges_and_vertices && data->neighbor_count < 4 && data->vertex_is_at_nonperiodic_boundary[vertex_i])
        continue;

      // (!!!) Find out u_i
      Vector<double> u_i(Equations<equationsType, dim>::n_components);
      const Point<dim> p_cell = this->mapping.transform_real_to_unit_cell(cell, data->vertexPoint[vertex_i]);
      const Quadrature<dim> one_point_quadrature(GeometryInfo<dim>::project_to_unit_cell(p_cell));
      FEValues<dim> fe_values(this->mapping, this->fe, one_point_quadrature, update_values);
      fe_values.reinit(cell);
      std::vector<Vector<double> > u_value(1, Vector<double>(this->fe.n_components()));
      fe_values.get_function_values(current_unlimited_solution, u_value);
      u_i = u_value[0];

      if (this->parameters.debug & this->parameters.SlopeLimiting)
      {
        LOGL(3, "\tv_i: " << cell->vertex(vertex_i) << ", values: ");
        for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
          LOGL(4, u_i[i] << (i == Equations<equationsType, dim>::n_components - 1 ? "" : ", "));
      }

      // Init u_i_min, u_i_max
      double u_i_min[Equations<equationsType, dim>::n_components];
      double u_i_max[Equations<equationsType, dim>::n_components];
      for (int k = 0; k < Equations<equationsType, dim>::n_components; k++)
      {
        u_i_min[k] = u_c[k];
        u_i_max[k] = u_c[k];
      }

      // For all vertices -> v_i
      for (auto dof_indices_neighbor : data->neighbor_dof_indices[vertex_i])
      {
        if (dof_indices_neighbor.size() == 0)
          continue;
        bool u_i_extrema_set[Equations<equationsType, dim>::n_components];
        for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
          u_i_extrema_set[i] = false;

        for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
        {
          if (this->is_primitive[i])
          {
            // Here we rely on the fact, that the constant basis fn is the first one.
            if (!u_i_extrema_set[this->component_ii[i]])
            {
              double val = current_unlimited_solution(dof_indices_neighbor[i]);
              if (this->parameters.debug & this->parameters.SlopeLimiting)
              {
                if (val < u_i_min[this->component_ii[i]])
                  LOGL(3, "\tdecreasing u_i_min to: " << val);
                if (val > u_i_max[this->component_ii[i]])
                  LOGL(3, "\tincreasing u_i_max to: " << val);
              }
              u_i_min[this->component_ii[i]] = std::min(u_i_min[this->component_ii[i]], val);
              u_i_max[this->component_ii[i]] = std::max(u_i_max[this->component_ii[i]], val);
              u_i_extrema_set[this->component_ii[i]] = true;
            }
          }
        }
      }

      // Based on u_i_min, u_i_max, u_i, get alpha_e
      for (int k = 0; k < Equations<equationsType, dim>::n_components; k++)
      {
        if (std::abs(u_c[k]) < SMALL)
          continue;
        if (std::abs((u_c[k] - u_i[k]) / u_c[k]) > NEGLIGIBLE)
        {
          alpha_e[k] = std::min(alpha_e[k], ((u_i[k] - u_c[k]) > 0.) ? std::min(1.0, (u_i_max[k] - u_c[k]) / (u_i[k] - u_c[k])) : std::min(1.0, (u_i_min[k] - u_c[k]) / (u_i[k] - u_c[k])));
          if (this->parameters.debug & this->parameters.SlopeLimiting)
            LOGL(1, "\talpha_e[" << k << "]: " << alpha_e[k]);
        }
      }
    }
	
    for (int k = 0; k < 5; k++)
      for (int i = 0; i < data->lambda_indices_to_multiply[k].size(); i++)
        current_limited_solution(data->lambda_indices_to_multiply[k][i]) *= alpha_e[k];

    if (this->parameters.limitB)
    {
      double mag_alpha = std::min(std::min(alpha_e[5], alpha_e[6]), alpha_e[7]);
      for (int k = 5; k < Equations<equationsType, dim>::n_components; k++)
        for (int i = 0; i < data->lambda_indices_to_multiply[k].size(); i++)
          current_limited_solution(data->lambda_indices_to_multiply[k][i]) *= mag_alpha;

      for (int i = 0; i < data->lambda_indices_to_multiply_all_B_components.size(); i++)
        current_limited_solution(data->lambda_indices_to_multiply_all_B_components[i]) *= mag_alpha;
    }
  }
}

template <EquationsType equationsType, int dim>
void BarthJespersenSlopeLimiter<equationsType, dim>::flush_cache()
{
  this->postprocessData.clear();
}

template <EquationsType equationsType, int dim>
void BarthJespersenSlopeLimiter<equationsType, dim>::postprocess(TrilinosWrappers::MPI::Vector& current_limited_solution, TrilinosWrappers::MPI::Vector& current_unlimited_solution)
{
  int cell_count = 0;
  // Loop through all cells.
  for (typename DoFHandler<dim>::active_cell_iterator cell = this->dof_handler.begin_active(); cell != this->dof_handler.end(); ++cell)
  {
    if (!cell->is_locally_owned())
      continue;

    bool u_c_set[Equations<equationsType, dim>::n_components];
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      u_c_set[i] = false;

    double u_c[Equations<equationsType, dim>::n_components];
    cell->get_dof_indices(this->dof_indices);

    typename SlopeLimiter<equationsType, dim>::PostprocessData* data = 0;
    auto it = this->postprocessData.find(cell->active_cell_index());
    if (it != this->postprocessData.end())
      data = &(it->second);
    else
    {
      data = &(((this->postprocessData.insert(std::make_pair(cell->active_cell_index(), typename SlopeLimiter<equationsType, dim>::PostprocessData()))).first)->second);
      for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
        u_c_set[i] = false;
      for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
      {
        if (!this->is_primitive[i])
          data->lambda_indices_to_multiply_all_B_components.push_back(this->dof_indices[i]);
        else
        {
          // Here we rely on the fact, that the constant basis fn is the first one and all other basis fns come after.
          if (!u_c_set[this->component_ii[i]])
            u_c_set[this->component_ii[i]] = true;
          else
            data->lambda_indices_to_multiply[this->component_ii[i]].push_back(this->dof_indices[i]);
        }
      }

      data->center = cell->center();
      for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
      {
        data->vertexIndex[vertex_i] = cell->vertex_index(vertex_i);
        data->vertexPoint[vertex_i] = data->center + (1. - NEGLIGIBLE) * (cell->vertex(vertex_i) - data->center);

        unsigned short neighbor_i = 0;

        for (auto neighbor_element : GridTools::find_cells_adjacent_to_vertex(this->triangulation, data->vertexIndex[vertex_i]))
        {
          typename DoFHandler<dim>::active_cell_iterator neighbor(&this->triangulation, neighbor_element->level(), neighbor_element->index(), &this->dof_handler);
          if (neighbor->active_cell_index() != cell->active_cell_index())
          {
            data->neighbor_dof_indices[vertex_i][neighbor_i].resize(this->dofs_per_cell);
            neighbor->get_dof_indices(data->neighbor_dof_indices[vertex_i][neighbor_i++]);
          }
        }
      }
    }

    // Cell center value we must find in any case (new data or reused)
    // - let us reuse this array for that.
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      u_c_set[i] = false;
    for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
    {
      if (this->is_primitive[i])
      {
        // Here we rely on the fact, that the constant basis fn is the first one and that all other basis fns have zero mean.
        if (!u_c_set[this->component_ii[i]])
        {
          u_c[this->component_ii[i]] = current_unlimited_solution(this->dof_indices[i]);
          u_c_set[this->component_ii[i]] = true;
        }
      }
    }

    if (this->parameters.debug & this->parameters.SlopeLimiting)
      LOGL(2, "cell: " << ++cell_count << " - center: " << data->center << ", values: " << u_c[0] << ", " << u_c[1] << ", " << u_c[2] << ", " << u_c[3] << ", " << u_c[4]);

    double alpha_e[Equations<equationsType, dim>::n_components];
    for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
      alpha_e[i] = 1.;
    // Init u_i_min, u_i_max
    double u_i_min[Equations<equationsType, dim>::n_components];
    double u_i_max[Equations<equationsType, dim>::n_components];
    for (int k = 0; k < Equations<equationsType, dim>::n_components; k++)
    {
      u_i_min[k] = u_c[k];
      u_i_max[k] = u_c[k];
    }

    for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
    {
      // For all vertices -> v_i
      for (auto dof_indices_neighbor : data->neighbor_dof_indices[vertex_i])
      {
        if (dof_indices_neighbor.size() == 0)
          continue;
        bool u_i_extrema_set[Equations<equationsType, dim>::n_components];
        for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
          u_i_extrema_set[i] = false;

        for (unsigned int i = 0; i < this->dofs_per_cell; ++i)
        {
          if (this->is_primitive[i])
          {
            // Here we rely on the fact, that the constant basis fn is the first one.
            if (!u_i_extrema_set[this->component_ii[i]])
            {
              double val = current_unlimited_solution(dof_indices_neighbor[i]);
              if (this->parameters.debug & this->parameters.SlopeLimiting)
              {
                if (val < u_i_min[this->component_ii[i]])
                  LOGL(3, "\tdecreasing u_i_min to: " << val);
                if (val > u_i_max[this->component_ii[i]])
                  LOGL(3, "\tincreasing u_i_max to: " << val);
              }
              u_i_min[this->component_ii[i]] = std::min(u_i_min[this->component_ii[i]], val);
              u_i_max[this->component_ii[i]] = std::max(u_i_max[this->component_ii[i]], val);
              u_i_extrema_set[this->component_ii[i]] = true;
            }
          }
        }
      }
    }

    // Based on u_i_min, u_i_max, u_i, get alpha_e
    for (unsigned int vertex_i = 0; vertex_i < GeometryInfo<dim>::vertices_per_cell; ++vertex_i)
    {
      // (!!!) Find out u_i
      Vector<double> u_i(Equations<equationsType, dim>::n_components);
      const Point<dim> p_cell = this->mapping.transform_real_to_unit_cell(cell, data->vertexPoint[vertex_i]);
      const Quadrature<dim> one_point_quadrature(GeometryInfo<dim>::project_to_unit_cell(p_cell));
      FEValues<dim> fe_values(this->mapping, this->fe, one_point_quadrature, update_values);
      fe_values.reinit(cell);
      std::vector<Vector<double> > u_value(1, Vector<double>(this->fe.n_components()));
      fe_values.get_function_values(current_unlimited_solution, u_value);
      u_i = u_value[0];

      if (this->parameters.debug & this->parameters.SlopeLimiting)
      {
        LOGL(3, "\tv_i: " << cell->vertex(vertex_i) << ", values: ");
        for (int i = 0; i < Equations<equationsType, dim>::n_components; i++)
          LOGL(4, u_i[i] << (i == Equations<equationsType, dim>::n_components - 1 ? "" : ", "));
      }

      for (int k = 0; k < Equations<equationsType, dim>::n_components; k++)
        if (std::abs((u_c[k] - u_i[k]) / u_c[k]) > NEGLIGIBLE)
        {
          alpha_e[k] = std::min(alpha_e[k], ((u_i[k] - u_c[k]) > 0.) ? std::min(1.0, (u_i_max[k] - u_c[k]) / (u_i[k] - u_c[k])) : std::min(1.0, (u_i_min[k] - u_c[k]) / (u_i[k] - u_c[k])));
          if (this->parameters.debug & this->parameters.SlopeLimiting)
            LOGL(5, "\talpha_e[" << k << "]: " << alpha_e[k]);
        }
    }

    for (int k = 0; k < Equations<equationsType, dim>::n_components; k++)
      for (int i = 0; i < data->lambda_indices_to_multiply[k].size(); i++)
        current_limited_solution(data->lambda_indices_to_multiply[k][i]) *= alpha_e[k];

    double alpha_e_B = std::min(std::min(alpha_e[5], alpha_e[6]), alpha_e[7]);
    for (int i = 0; i < data->lambda_indices_to_multiply_all_B_components.size(); i++)
      current_limited_solution(data->lambda_indices_to_multiply_all_B_components[i]) *= alpha_e_B;
  }
}

template class SlopeLimiter<EquationsTypeMhd, 3>;
template class VertexBasedSlopeLimiter<EquationsTypeMhd, 3>;
template class BarthJespersenSlopeLimiter<EquationsTypeMhd, 3>;
