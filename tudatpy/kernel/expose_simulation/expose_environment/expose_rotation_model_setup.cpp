/*    Copyright (c) 2010-2019, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "expose_rotation_model_setup.h"

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
// namespace te = tudat::ephemerides;
// namespace ti = tudat::interpolators;
namespace tba = tudat::basic_astrodynamics;
// namespace ta = tudat::aerodynamics;
// namespace trf = tudat::reference_frames;
// namespace tg = tudat::gravitation;
// namespace tcc = tudat::coordinate_conversions;

namespace tudatpy {

void expose_rotation_model_setup(py::module &m) {
    /////////////////////////////////////////////////////////////////////////////
    // createRotationalModel.h
    /////////////////////////////////////////////////////////////////////////////
    py::enum_<tss::RotationModelType>(m, "RotationModelType", "<no_doc>")
            .value("simple_rotational_model",
                   tss::RotationModelType::simple_rotation_model)
            .value("spice_rotation_model",
                   tss::RotationModelType::spice_rotation_model)
            .value("gcrs_to_itrs_rotation_model",
                   tss::RotationModelType::gcrs_to_itrs_rotation_model)
            .value("synchronous_rotation_model",
                   tss::RotationModelType::synchronous_rotation_model)
            .value("planetary_rotation_model",
                   tss::RotationModelType::planetary_rotation_model)
            .export_values();

    py::enum_<tba::IAUConventions>(m, "IAUConventions", "<no_doc>")
            .value("iau_2000_a",
                   tba::IAUConventions::iau_2000_a)
            .value("iau_2000_b",
                   tba::IAUConventions::iau_2000_b)
            .value("iau_2006",
                   tba::IAUConventions::iau_2006)
            .export_values();

    py::class_<tss::RotationModelSettings,
            std::shared_ptr<tss::RotationModelSettings>>(
            m, "RotationalModelSettings", "<no doc>")
//            .def(py::init<const tss::RotationModelType, const std::string &,
//                 const std::string &>(),
//                 py::arg("rotation_type"), py::arg("base_frame"),
//                 py::arg("target_frame"))
            .def_property_readonly("rotation_type", &tss::RotationModelSettings::getRotationType)
            .def_property("base_frame", &tss::RotationModelSettings::getOriginalFrame, &tss::RotationModelSettings::resetOriginalFrame)
            .def_property_readonly("target_frame", &tss::RotationModelSettings::getTargetFrame);


    m.def("simple",
          py::overload_cast< const std::string&, const std::string& ,
                  const Eigen::Matrix3d&, const double, const double >( &tss::simpleRotationModelSettings ),
          py::arg("base_frame"),
          py::arg("target_frame"),
          py::arg("initial_orientation"),
          py::arg("initial_time"),
          py::arg("rotation_rate"),
          get_docstring("simple").c_str()
    );

    m.def("simple_from_spice",
          &tss::simpleRotationModelFromSpiceSettings,
          py::arg("base_frame"),
          py::arg("target_frame"),
          py::arg("target_frame_spice"),
          py::arg("initial_time"),
          get_docstring("simple_from_spice").c_str()
    );

    m.def("synchronous",
          &tss::synchronousRotationModelSettings,
          py::arg("central_body_name"),
          py::arg("base_frame"),
          py::arg("target_frame"),
          get_docstring("synchronous").c_str()
    );

    m.def("spice",
          &tss::spiceRotationModelSettings,
          py::arg("base_frame"),
          py::arg("target_frame"),
          get_docstring("spice").c_str()
    );

    m.def("gcrs_to_itrs",
          &tss::gcrsToItrsRotationModelSettings,
          py::arg("precession_nutation_theory") = tba::iau_2006,
          py::arg("base_frame") = "GCRS",
          get_docstring("gcrs_to_itrs").c_str()
    );

    m.def("constant",
          py::overload_cast< const std::string&, const std::string&, const Eigen::Matrix3d& >(
                  &tss::constantRotationModelSettings ),
          py::arg("base_frame"),
          py::arg("target_frame"),
          py::arg("initial_orientation"),
          get_docstring("constant").c_str());

}

} // namespace tudatpy