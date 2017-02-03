template typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type Equations<EquationsTypeEuler, 2>::compute_kinetic_energy(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type Equations<EquationsTypeEuler, 2>::compute_pressure(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W) const;

template void Equations<EquationsTypeEuler, 2>::compute_flux_matrix(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W, std_cxx11::array <std_cxx11::array <typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, 2>, n_components > &flux) const;

template void Equations<EquationsTypeEuler, 2>::compute_jacobian_addition(double, const dealii::internal::TableBaseAccessors::Accessor<3, double, false, 2> &W, std_cxx11::array <std_cxx11::array<double, 2>, n_components> &jacobian_addition) const;

template void Equations<EquationsTypeEuler, 2>::numerical_normal_flux(const Tensor<1, 2> &normal, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wplus, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wminus, 
  std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, n_components> &normal_flux) const;

template void Equations<EquationsTypeEuler, 2>::compute_forcing_vector(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W, std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, n_components> &forcing) const;

template void Equations<EquationsTypeEuler, 2>::compute_Wminus(const BoundaryKind(&boundary_kind)[n_components], const Tensor<1, 2> &normal_vector, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wplus, const Vector<double> &boundary_values, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wminus) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type Equations<EquationsTypeEuler, 3>::compute_kinetic_energy(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type Equations<EquationsTypeEuler, 3>::compute_pressure(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W) const;

template void Equations<EquationsTypeEuler, 3>::compute_flux_matrix(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W, std_cxx11::array <std_cxx11::array <typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, 3>, n_components > &flux) const;

template void Equations<EquationsTypeEuler, 3>::compute_jacobian_addition(double, const dealii::internal::TableBaseAccessors::Accessor<3, double, false, 2> &W, std_cxx11::array <std_cxx11::array <double, 3>, n_components > &jacobian_addition) const;

template void Equations<EquationsTypeEuler, 3>::numerical_normal_flux(const Tensor<1, 3> &normal, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wplus, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wminus, 
  std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, n_components> &normal_flux) const;

template void Equations<EquationsTypeEuler, 3>::compute_forcing_vector(const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &W, std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1>::value_type, n_components> &forcing) const;

template void Equations<EquationsTypeEuler, 3>::compute_Wminus(const BoundaryKind(&boundary_kind)[n_components], const Tensor<1, 3> &normal_vector, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wplus, const Vector<double> &boundary_values, const dealii::internal::TableBaseAccessors::Accessor<2, double, false, 1> &Wminus) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type Equations<EquationsTypeEuler, 2>::compute_kinetic_energy(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type Equations<EquationsTypeEuler, 2>::compute_pressure(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W) const;

template void Equations<EquationsTypeEuler, 2>::compute_flux_matrix(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &W, std_cxx11::array <std_cxx11::array <typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1>::value_type, 2>, n_components > &flux) const;

template void Equations<EquationsTypeEuler, 2>::compute_jacobian_addition(double, const dealii::internal::TableBaseAccessors::Accessor<3, Sacado::Fad::DFad<double>, false, 2> &W, std_cxx11::array <std_cxx11::array <Sacado::Fad::DFad<double>, 2>, n_components > &jacobian_addition) const;

template void Equations<EquationsTypeEuler, 2>::numerical_normal_flux(const Tensor<1, 2> &normal, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &Wplus, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &Wminus, 
  std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type, n_components> &normal_flux) const;

template void Equations<EquationsTypeEuler, 2>::compute_forcing_vector(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W, std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type, n_components> &forcing) const;

template void Equations<EquationsTypeEuler, 2>::compute_Wminus(const BoundaryKind(&boundary_kind)[n_components], const Tensor<1, 2> &normal_vector, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &Wplus, const Vector<double> &boundary_values, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &Wminus) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type Equations<EquationsTypeEuler, 3>::compute_kinetic_energy(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W) const;

template typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type Equations<EquationsTypeEuler, 3>::compute_pressure(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W) const;

template void Equations<EquationsTypeEuler, 3>::compute_flux_matrix(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &W, std_cxx11::array <std_cxx11::array <typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1>::value_type, 3>, n_components > &flux) const;

template void Equations<EquationsTypeEuler, 3>::compute_jacobian_addition(double, const dealii::internal::TableBaseAccessors::Accessor<3, Sacado::Fad::DFad<double> , false, 2> &W, std_cxx11::array <std_cxx11::array <Sacado::Fad::DFad<double>, 3>, n_components > &jacobian_addition) const;

template void Equations<EquationsTypeEuler, 3>::numerical_normal_flux(const Tensor<1, 3> &normal, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &Wplus, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &Wminus,
  std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type, n_components> &normal_flux) const;

template void Equations<EquationsTypeEuler, 3>::compute_forcing_vector(const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1> &W, std_cxx11::array<typename dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double> , false, 1>::value_type, n_components> &forcing) const;

template void Equations<EquationsTypeEuler, 3>::compute_Wminus(const BoundaryKind(&boundary_kind)[n_components], const Tensor<1, 3> &normal_vector, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &Wplus, const Vector<double> &boundary_values, const dealii::internal::TableBaseAccessors::Accessor<2, Sacado::Fad::DFad<double>, false, 1> &Wminus) const;