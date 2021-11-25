/*    Copyright (c) 2010-2021, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "expose_acceleration_setup.h"

#include "tudatpy/docstrings.h"
#include <tudat/simulation/propagation_setup.h>

#include <pybind11/chrono.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
namespace tba = tudat::basic_astrodynamics;
namespace tss = tudat::simulation_setup;
namespace tp = tudat::propagators;
namespace tinterp = tudat::interpolators;
namespace te = tudat::ephemerides;
namespace tni = tudat::numerical_integrators;
namespace trf = tudat::reference_frames;
namespace tmrf = tudat::root_finders;

namespace tudat {
namespace simulation_setup {

//! @get_docstring(cannonBallRadiationPressureAcceleration)
inline std::shared_ptr< AccelerationSettings > panelledRadiationPressureAcceleration( )
{
    return std::make_shared< AccelerationSettings >( basic_astrodynamics::panelled_radiation_pressure_acceleration );
}

//! @get_docstring(customAccelerationSettings)
inline std::shared_ptr< AccelerationSettings > customAccelerationSettings(
        const std::function< Eigen::Vector3d( const double ) > accelerationFunction )
{
        return std::make_shared< CustomAccelerationSettings >(
                    accelerationFunction );

}

}
}

namespace tudatpy {
namespace numerical_simulation {
namespace propagation_setup {
namespace acceleration {

void expose_acceleration_setup(py::module &m) {

    /*
     * This contains the addition of IntegratorSettings and AvailableIntegrators
     * and AvailableAccelerations which should be relocated in the tudat source.
     */


    py::enum_<tba::AvailableAcceleration>(m, "AvailableAcceleration", get_docstring("AvailableAcceleration").c_str())
            .value("undefined_acceleration_type", tba::AvailableAcceleration::undefined_acceleration, get_docstring("AvailableAcceleration.undefined_acceleration_type").c_str())
            .value("point_mass_gravity_type", tba::AvailableAcceleration::point_mass_gravity, get_docstring("AvailableAcceleration.point_mass_gravity_type").c_str())
            .value("aerodynamic_type", tba::AvailableAcceleration::aerodynamic, get_docstring("AvailableAcceleration.aerodynamic_type").c_str())
            .value("cannonball_radiation_pressure_type", tba::AvailableAcceleration::cannon_ball_radiation_pressure, get_docstring("AvailableAcceleration.cannon_ball_radiation_pressure_type").c_str())
            .value("spherical_harmonic_gravity_type", tba::AvailableAcceleration::spherical_harmonic_gravity, get_docstring("AvailableAcceleration.spherical_harmonic_gravity_type").c_str())
            .value("mutual_spherical_harmonic_gravity_type", tba::AvailableAcceleration::mutual_spherical_harmonic_gravity, get_docstring("AvailableAcceleration.mutual_spherical_harmonic_gravity_type").c_str())
            .value("thrust_acceleration_type", tba::AvailableAcceleration::thrust_acceleration, get_docstring("AvailableAcceleration.thrust_acceleration_type").c_str())
            .value("relativistic_correction_acceleration_type", tba::AvailableAcceleration::relativistic_correction_acceleration, get_docstring("AvailableAcceleration.relativistic_correction_acceleration_type").c_str())
            .value("empirical_acceleration_type", tba::AvailableAcceleration::empirical_acceleration, get_docstring("AvailableAcceleration.empirical_acceleration_type").c_str())
            .value("direct_tidal_dissipation_in_central_body_acceleration_type", tba::AvailableAcceleration::direct_tidal_dissipation_in_central_body_acceleration, get_docstring("AvailableAcceleration.direct_tidal_dissipation_in_central_body_acceleration_type").c_str())
            .value("direct_tidal_dissipation_in_orbiting_body_acceleration_type", tba::AvailableAcceleration::direct_tidal_dissipation_in_orbiting_body_acceleration, get_docstring("AvailableAcceleration.direct_tidal_dissipation_in_orbiting_body_acceleration_type").c_str())
            .value("panelled_radiation_pressure_acceleration_type", tba::AvailableAcceleration::panelled_radiation_pressure_acceleration, get_docstring("AvailableAcceleration.panelled_radiation_pressure_acceleration_type").c_str())
            .value("quasi_impulsive_shots_acceleration_type", tba::AvailableAcceleration::momentum_wheel_desaturation_acceleration, get_docstring("AvailableAcceleration.momentum_wheel_desaturation_acceleration_type").c_str())
            .value("solar_sail_acceleration_type", tba::AvailableAcceleration::solar_sail_acceleration, get_docstring("AvailableAcceleration.solar_sail_acceleration_type").c_str())
            .export_values();

    //////////////////////////////////////////////////////////////////////////////
    // accelerationSettings.h
    //////////////////////////////////////////////////////////////////////////////

    py::class_<tss::AccelerationSettings,
            std::shared_ptr<tss::AccelerationSettings>>(m, "AccelerationSettings",
                                                        get_docstring("AccelerationSettings").c_str());
//            .def(py::init<const tudat::basic_astrodynamics::AvailableAcceleration>(),
//                 py::arg("acceleration_type"));

    py::class_<tss::SphericalHarmonicAccelerationSettings,
            std::shared_ptr<tss::SphericalHarmonicAccelerationSettings>,
            tss::AccelerationSettings>(m, "SphericalHarmonicAccelerationSettings",
                                       get_docstring("SphericalHarmonicAccelerationSettings").c_str());
//            .def(py::init<const int, const int>(), py::arg("maximum_degree"),
//                 py::arg("maximum_order"));

    py::class_<tss::MutualSphericalHarmonicAccelerationSettings,
            std::shared_ptr<tss::MutualSphericalHarmonicAccelerationSettings>,
            tss::AccelerationSettings>(m, "MutualSphericalHarmonicAccelerationSettings",
                                       get_docstring("MutualSphericalHarmonicAccelerationSettings").c_str());


    py::class_<tss::EmpiricalAccelerationSettings,
            std::shared_ptr<tss::EmpiricalAccelerationSettings>,
            tss::AccelerationSettings>(m, "EmpiricalAccelerationSettings",
                                       get_docstring("EmpiricalAccelerationSettings").c_str());


    py::class_<tss::RelativisticAccelerationCorrectionSettings,
            std::shared_ptr<tss::RelativisticAccelerationCorrectionSettings>,
            tss::AccelerationSettings>(m, "RelativisticAccelerationCorrectionSettings",
                                       get_docstring("RelativisticAccelerationCorrectionSettings").c_str());


    py::class_<tss::CustomAccelerationSettings,
            std::shared_ptr<tss::CustomAccelerationSettings>,
            tss::AccelerationSettings>(m, "CustomAccelerationSettings",
                                       get_docstring("CustomAccelerationSettings").c_str());


    py::class_<tss::DirectTidalDissipationAccelerationSettings,
            std::shared_ptr<tss::DirectTidalDissipationAccelerationSettings>,
            tss::AccelerationSettings>(m, "DirectTidalDissipationAcceleration",
                                       get_docstring("DirectTidalDissipationAcceleration").c_str());

    py::class_<tss::MomentumWheelDesaturationAccelerationSettings,
            std::shared_ptr<tss::MomentumWheelDesaturationAccelerationSettings>,
            tss::AccelerationSettings>(m, "MomentumWheelDesaturationAcceleration",
                                       get_docstring("MomentumWheelDesaturationAcceleration").c_str());

    py::class_<tss::ThrustAccelerationSettings,
            std::shared_ptr<tss::ThrustAccelerationSettings>,
            tss::AccelerationSettings>(m, "ThrustAccelerationSettings",
                                       get_docstring("ThrustAccelerationSettings").c_str())
//            .def(py::init<//ctor 1
//                         const std::shared_ptr<tss::ThrustDirectionSettings>,
//                         const std::shared_ptr<tss::ThrustMagnitudeSettings>>(),
//                 py::arg("thrust_direction_settings"),
//                 py::arg("thrust_magnitude_settings"))
//            .def(py::init<//ctor 2
//                         const std::shared_ptr<tinterp::DataInterpolationSettings<double, Eigen::Vector3d>> &,
//                         const std::function<double(const double)>,
//                         const tss::ThrustFrames,
//                         const std::string>(),
//                 py::arg("data_interpolation_settings"),
//                 py::arg("specific_impulse_function"),
//                 py::arg("thrust_frame"),
//                 py::arg("central_body") = "")
//            .def(py::init<//ctor 3
//                         const std::shared_ptr<tinterp::DataInterpolationSettings<double, Eigen::Vector3d>> &,
//                         const double,
//                         const tss::ThrustFrames,
//                         const std::string>(),
//                 py::arg("data_interpolation_settings"),
//                 py::arg("constant_specific_impulse"),
//                 py::arg("thrust_frame"),
//                 py::arg("central_body") = "")
            .def_readwrite("direction_settings", &tss::ThrustAccelerationSettings::thrustMagnitudeSettings_ )
            .def_readwrite("magnitude_settings", &tss::ThrustAccelerationSettings::thrustDirectionSettings_ );


    // Unified interface functions for acceleration settings
    //  m.def("acceleration", &tss::acceleration, py::arg("acceleration_type"));
    m.def("point_mass_gravity", &tss::pointMassGravityAcceleration,
          get_docstring("point_mass_gravity").c_str());

    m.def("aerodynamic", &tss::aerodynamicAcceleration,
          get_docstring("aerodynamic").c_str());

    m.def("cannonball_radiation_pressure", &tss::cannonBallRadiationPressureAcceleration,
          get_docstring("cannonball_radiation_pressure").c_str());

    m.def("panelled_radiation_pressure", &tss::panelledRadiationPressureAcceleration,
          get_docstring("panelled_radiation_pressure").c_str());


    m.def("spherical_harmonic_gravity", &tss::sphericalHarmonicAcceleration,
          py::arg( "maximum_degree" ),
          py::arg( "maximum_order" ),
          get_docstring("spherical_harmonic_gravity").c_str());

    m.def("mutual_spherical_harmonic_gravity", &tss::mutualSphericalHarmonicAcceleration,
          py::arg( "maximum_degree_body_exerting" ),
          py::arg( "maximum_order_body_exerting" ),
          py::arg( "maximum_degree_body_undergoing" ),
          py::arg( "maximum_order_body_undergoing" ),
          py::arg( "maximum_degree_central_body" ) = 0,
          py::arg( "maximum_order_central_body" ) = 0,
          get_docstring("mutual_spherical_harmonic_gravity").c_str());

    m.def("relativistic_correction", &tss::relativisticAccelerationCorrection,
          py::arg( "use_schwarzschild" ) = false,
          py::arg( "use_lense_thirring" ) = false,
          py::arg( "use_de_sitter" ) = false,
          py::arg( "de_sitter_central_body" ) = "",
          py::arg( "lense_thirring_angular_momentum" ) = Eigen::Vector3d::Zero( ),
          get_docstring("relativistic_correction").c_str());

    m.def("empirical", &tss::empiricalAcceleration,
          py::arg( "constant_acceleration" ) = Eigen::Vector3d::Zero( ),
          py::arg( "sine_acceleration" ) = Eigen::Vector3d::Zero( ),
          py::arg( "cosine_acceleration" ) = Eigen::Vector3d::Zero( ),
          get_docstring("empirical").c_str());

    m.def("custom",
          py::overload_cast< std::function< Eigen::Vector3d( const double ) > >(
              &tss::customAccelerationSettings ),
          py::arg( "acceleration_function" ),
          get_docstring("custom").c_str());

    m.def("direct_tidal_dissipation_acceleration", &tss::directTidalDissipationAcceleration,
          py::arg("k2_love_number"),
          py::arg("time_lag"),
          py::arg("include_direct_radial_component") = true,
          py::arg("use_tide_raised_on_planet") = true,
          get_docstring("direct_tidal_dissipation_acceleration").c_str());

    m.def("quasi_impulsive_shots_acceleration", &tss::momentumWheelDesaturationAcceleration,
          py::arg("thrust_mid_times"),
          py::arg("delta_v_values"),
          py::arg("total_maneuver_time"),
          py::arg("maneuver_rise_time"),
          get_docstring("quasi_impulsive_shots_acceleration").c_str());

    m.def("thrust_from_direction_and_magnitude", py::overload_cast<const std::shared_ptr<tss::ThrustDirectionSettings>,
                  const std::shared_ptr<tss::ThrustMagnitudeSettings>>(&tss::thrustAcceleration),
          py::arg("thrust_direction_settings"),
          py::arg("thrust_magnitude_settings"),
          get_docstring("thrust_from_direction_and_magnitude").c_str());

    m.def("thrust_from_custom_function", py::overload_cast<
                  const std::function< Eigen::Vector3d( const double ) >,
                  const std::function<double(const double)>,
                  const trf::SatelliteReferenceFrames,
                  const std::string>(&tss::thrustAcceleration),
          py::arg("thrust_force_function"),
          py::arg("specific_impulse_function"),
          py::arg("thrust_frame") = trf::SatelliteReferenceFrames::global_reference_frame,
          py::arg("central_body") = "",
          get_docstring("thrust_from_custom_function").c_str());

    m.def("thrust_and_isp_from_custom_function", py::overload_cast<
                  const std::function< Eigen::Vector3d( const double ) >,
                  const double,
                  const trf::SatelliteReferenceFrames,
                  const std::string>(&tss::thrustAcceleration),
          py::arg("thrust_force_function"),
          py::arg("constant_specific_impulse"),
          py::arg("thrust_frame") = trf::SatelliteReferenceFrames::global_reference_frame,
          py::arg("central_body") = "",
          get_docstring("thrust_and_isp_from_custom_function").c_str());


}

}// namespace acceleration
}// namespace propagation_setup
}// namespace numerical_simulation
}// namespace tudatpy
