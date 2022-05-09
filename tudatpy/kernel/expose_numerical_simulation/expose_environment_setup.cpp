/*    Copyright (c) 2010-2019, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */


#include "expose_environment_setup.h"

#include "expose_environment_setup/expose_aerodynamic_coefficient_setup.h"
#include "expose_environment_setup/expose_atmosphere_setup.h"
#include "expose_environment_setup/expose_ephemeris_setup.h"
#include "expose_environment_setup/expose_gravity_field_setup.h"
#include "expose_environment_setup/expose_gravity_field_variation_setup.h"
#include "expose_environment_setup/expose_radiation_pressure_setup.h"
#include "expose_environment_setup/expose_rotation_model_setup.h"
#include "expose_environment_setup/expose_shape_setup.h"

#include "tudatpy/docstrings.h"
#include <tudat/simulation/environment_setup.h>
#include <tudat/astro/reference_frames/referenceFrameTransformations.h>

//#include <pybind11/chrono.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
//#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>

namespace py = pybind11;
namespace tss = tudat::simulation_setup;
namespace te = tudat::ephemerides;
namespace ti = tudat::interpolators;
namespace tba = tudat::basic_astrodynamics;
namespace ta = tudat::aerodynamics;
namespace trf = tudat::reference_frames;
namespace tg = tudat::gravitation;
namespace tcc = tudat::coordinate_conversions;


namespace tudat
{
namespace simulation_setup
{

void setGuidanceAnglesFunctionsPy(
        const std::shared_ptr< ta::AerodynamicGuidance > aerodynamicGuidance,
        const std::shared_ptr< tss::Body > bodyWithAngles,
        const bool silenceWarnings )
{
    std::shared_ptr< reference_frames::DependentOrientationCalculator >  orientationCalculator =
            bodyWithAngles->getDependentOrientationCalculator( );
    std::shared_ptr< reference_frames::AerodynamicAngleCalculator > angleCalculator =
            std::dynamic_pointer_cast< reference_frames::AerodynamicAngleCalculator >( orientationCalculator );

    if( angleCalculator == nullptr )
    {
        throw std::runtime_error( "Error, body does not have AerodynamicAngleCalculator when setting aerodynamic guidance" );
    }
    else
    {
        setGuidanceAnglesFunctions( aerodynamicGuidance, angleCalculator, silenceWarnings );
    }
}

}

}


namespace tudatpy {
namespace numerical_simulation {
namespace environment_setup {

    void expose_environment_setup(py::module &m) {

//        m.def("get_body_gravitational_parameter",
//              &tss::getBodyGravitationalParameter,
//              py::arg("body_collection"), py::arg("body_name"));


        py::class_<tss::BodySettings, std::shared_ptr<tss::BodySettings>>(
                m, "BodySettings", get_docstring("BodySettings").c_str())
                .def_readwrite("constant_mass", &tss::BodySettings::constantMass, get_docstring("BodySettings.constant_mass").c_str())
                .def_readwrite("atmosphere_settings", &tss::BodySettings::atmosphereSettings, get_docstring("BodySettings.atmosphere_settings").c_str())
                .def_readwrite("ephemeris_settings", &tss::BodySettings::ephemerisSettings, get_docstring("BodySettings.ephemeris_settings").c_str())
                .def_readwrite("gravity_field_settings", &tss::BodySettings::gravityFieldSettings, get_docstring("BodySettings.gravity_field_settings").c_str())
                .def_readwrite("rotation_model_settings", &tss::BodySettings::rotationModelSettings, get_docstring("BodySettings.rotation_model_settings").c_str())
                .def_readwrite("shape_settings", &tss::BodySettings::shapeModelSettings, get_docstring("BodySettings.shape_settings").c_str())
                .def_readwrite("radiation_pressure_settings", &tss::BodySettings::radiationPressureSettings, get_docstring("BodySettings.radiation_pressure_settings").c_str())
                .def_readwrite("aerodynamic_coefficient_settings", &tss::BodySettings::aerodynamicCoefficientSettings, get_docstring("BodySettings.aerodynamic_coefficient_settings").c_str())
                .def_readwrite("gravity_field_variation_settings", &tss::BodySettings::gravityFieldVariationSettings, get_docstring("BodySettings.gravity_field_variation_settings").c_str());

        py::class_<tss::BodyListSettings,
                std::shared_ptr<tss::BodyListSettings> >(m, "BodyListSettings", get_docstring("BodyListSettings").c_str())
                .def("get", &tss::BodyListSettings::get, get_docstring("BodyListSettings.get").c_str())
                .def("add_settings", py::overload_cast<std::shared_ptr<tss::BodySettings>, const std::string>
                        (&tss::BodyListSettings::addSettings), py::arg("settings_to_add"), py::arg("body_name"))
                .def("add_empty_settings", py::overload_cast<const std::string>(&tss::BodyListSettings::addSettings),
                     py::arg("body_name"))
                .def_property_readonly("frame_origin", &tss::BodyListSettings::getFrameOrigin, get_docstring("BodyListSettings.frame_origin").c_str())
                .def_property_readonly("frame_orientation", &tss::BodyListSettings::getFrameOrientation, get_docstring("BodyListSettings.frame_orientation").c_str());

        m.def("get_default_body_settings",
              py::overload_cast<const std::vector<std::string> &, const std::string,
                      const std::string>(
                      &tss::getDefaultBodySettings),
              py::arg("bodies"),
              py::arg("base_frame_origin") = "SSB",
              py::arg("base_frame_orientation") = "ECLIPJ2000",
              get_docstring("get_default_body_settings").c_str());

        m.def("get_default_body_settings_time_limited",
              py::overload_cast<const std::vector<std::string> &,
                      const double, const double,
                      const std::string, const std::string, const double>(
                      &tss::getDefaultBodySettings),
              py::arg("bodies"),
              py::arg("initial_time"),
              py::arg("final_time"),
              py::arg("base_frame_origin") = "SSB",
              py::arg("base_frame_orientation") = "ECLIPJ2000",
              py::arg("time_step") = 300.0,
              get_docstring("get_default_body_settings_time_limited").c_str());

        m.def("get_default_single_body_settings",
              py::overload_cast<const std::string&, const std::string&>(
                      &tss::getDefaultSingleBodySettings),
              py::arg("body_name"),
              py::arg("base_frame_orientation") = "ECLIPJ2000",
              get_docstring("get_default_single_body_settings").c_str());

        m.def("get_default_single_body_settings_time_limited",
              py::overload_cast< const std::string&, const double, const double, const std::string&, const double >(
                  &tss::getDefaultSingleBodySettings),
              py::arg("body_name"),
              py::arg("initial_time"),
              py::arg("final_time"),
              py::arg("base_frame_orientation") = "ECLIPJ2000",
              py::arg("time_step") = 300.0,
              get_docstring("get_default_single_body_settings_time_limited").c_str());


        m.def("create_simplified_system_of_bodies", &tss::createSimplifiedSystemOfBodies,
              py::arg("initial_time") = 0,
              get_docstring("create_simplified_system_of_bodies").c_str());

        m.def("create_system_of_bodies", &tss::createSystemOfBodies,
              py::arg("body_settings"),
              get_docstring("create_system_of_bodies").c_str());

        m.def("add_empty_tabulated_ephemeris", &tss::addEmptyTabulatedEphemeris,
              py::arg("bodies"),
              py::arg("body_name"),
              py::arg("ephemeris_origin") = "",
              get_docstring("add_empty_tabulated_ephemeris").c_str());

        m.def("create_tabulated_ephemeris_from_spice",
                &tss::createTabulatedEphemerisFromSpice<>, py::arg("body"),
                py::arg("initial_time"), py::arg("end_time"), py::arg("time_step"),
                py::arg("observer_name"), py::arg("reference_frame_name"),
                py::arg("interpolator_settings") =
                        std::make_shared<tudat::interpolators::LagrangeInterpolatorSettings>(
                                8));

        m.def("create_body_ephemeris", &tss::createBodyEphemeris,
              py::arg("ephemeris_settings"), py::arg("body_name"));

        m.def("get_safe_interpolation_interval", &tss::getSafeInterpolationInterval,
              py::arg("ephemeris_model"));


        m.def("add_aerodynamic_coefficient_interface",
              &tss::addAerodynamicCoefficientInterface,
              py::arg("bodies"), py::arg("body_name"), py::arg("coefficient_settings"),
              get_docstring("add_aerodynamic_coefficient_interface").c_str());

        m.def("create_aerodynamic_coefficient_interface",
              &tss::createAerodynamicCoefficientInterface,
              py::arg("coefficient_settings"), py::arg("body"),
              get_docstring("create_aerodynamic_coefficient_interface").c_str());

        m.def("add_radiation_pressure_interface",
              &tss::addRadiationPressureInterface,
              py::arg("bodies"), py::arg("body_name"), py::arg("radiation_pressure_settings"),
              get_docstring("add_radiation_pressure_interface").c_str());

        m.def("add_flight_conditions",
              &tss::addFlightConditions,
              py::arg("bodies"), py::arg("body_name"), py::arg("central_body_name"),
              get_docstring("add_flight_conditions").c_str());

        m.def("add_ground_station",
              py::overload_cast<
                      const std::shared_ptr<tss::Body>,
                      const std::string,
                      const Eigen::Vector3d,
                      const tcc::PositionElementTypes>(&tss::createGroundStation),
              py::arg("body"),
              py::arg("ground_station_name"),
              py::arg("ground_station_position"),
              py::arg("position_type") = tcc::cartesian_position,
              get_docstring("add_ground_station").c_str());

        m.def("create_radiation_pressure_interface",
              &tss::createRadiationPressureInterface,
              py::arg("radiationPressureInterfaceSettings"), py::arg("body_name"),
              py::arg("body_dict"));

          m.def("set_aerodynamic_guidance",
              &tss::setGuidanceAnglesFunctionsPy,
              py::arg("aerodynamic_guidance"),
              py::arg("body"),
              py::arg("silence_warnings") = false );

        m.def("set_aerodynamic_orientation_functions", &tss::setAerodynamicOrientationFunctions,
              py::arg("body"),
              py::arg("angle_of_attack_function") = std::function<double()>(),
              py::arg("sideslip_angle_function") = std::function<double()>(),
              py::arg("bank_angle_function") = std::function<double()>(),
              py::arg("update_function") = std::function<void(const double)>());

        m.def("set_constant_aerodynamic_orientation", &tss::setConstantAerodynamicOrientation,
              py::arg("body"),
              py::arg("angle_of_attack"),
              py::arg("sideslip_angle"),
              py::arg("bank_angle"),
              py::arg("silence_warnings") = false );

        m.def("get_ground_station_list",
              &tss::getGroundStationsLinkEndList,
              py::arg( "body" ) );

        m.def("get_target_elevation_angles",
              &tss::getTargetElevationAngles,
              py::arg( "observing_body" ),
              py::arg( "target_body" ),
              py::arg( "station_name" ),
              py::arg( "times" ) );


        auto aerodynamic_coefficient_setup = m.def_submodule("aerodynamic_coefficients");
        aerodynamic_coefficients::expose_aerodynamic_coefficient_setup(aerodynamic_coefficient_setup);

        auto radiation_pressure_setup = m.def_submodule("radiation_pressure");
        radiation_pressure::expose_radiation_pressure_setup(radiation_pressure_setup);

        auto rotation_model_setup = m.def_submodule("rotation_model");
        rotation_model::expose_rotation_model_setup(rotation_model_setup);

        auto gravity_field_setup = m.def_submodule("gravity_field");
        gravity_field::expose_gravity_field_setup(gravity_field_setup);

        auto ephemeris_setup = m.def_submodule("ephemeris");
        ephemeris::expose_ephemeris_setup(ephemeris_setup);

        auto atmosphere_setup = m.def_submodule("atmosphere");
        atmosphere::expose_atmosphere_setup(atmosphere_setup);

        auto shape_setup = m.def_submodule("shape");
        shape::expose_shape_setup(shape_setup);

        auto gravity_variation_setup = m.def_submodule("gravity_field_variation");
        gravity_field_variation::expose_gravity_field_variation_setup(gravity_variation_setup);


    }

}// namespace environment_setup
}// namespace numerical_simulation
}// namespace tudatpy
