/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#include "expose_observations.h"

#include <stdio.h>
#include <time.h>

#include <tudat/astro/observation_models.h>
#include <tudat/simulation/estimation_setup/observations.h>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;
namespace tom = tudat::observation_models;


namespace tudatpy {

void expose_observations(py::module &m) {

    py::class_< tom::ObservationCollection<>,
            std::shared_ptr<tom::ObservationCollection<>>>(m, "ObservationCollection");

    py::class_<tom::ObservationViabilityCalculator,
               std::shared_ptr<tom::ObservationViabilityCalculator>>(m, "ObservationViabilityCalculator")
            .def("is_observation_viable", &tom::ObservationViabilityCalculator::isObservationViable,
                 py::arg( "link_end_states" ),
                 py::arg( "link_end_times" ) );

    py::class_<tom::ObservationSimulatorBase<double,double>,
               std::shared_ptr<tom::ObservationSimulatorBase<double,double>>>(m, "ObservationSimulator");

    py::class_<tom::ObservationSimulator<1,double,double>,
               std::shared_ptr<tom::ObservationSimulator<1,double,double>>,
               tom::ObservationSimulatorBase<double,double>>(m, "ObservationSimulator_1");

    py::class_<tom::ObservationSimulator<2,double,double>,
               std::shared_ptr<tom::ObservationSimulator<2,double,double>>,
               tom::ObservationSimulatorBase<double,double>>(m, "ObservationSimulator_2");

    py::class_<tom::ObservationSimulator<3,double,double>,
               std::shared_ptr<tom::ObservationSimulator<3,double,double>>,
               tom::ObservationSimulatorBase<double,double>>(m, "ObservationSimulator_3");

    py::class_<tom::ObservationSimulator<6,double,double>,
               std::shared_ptr<tom::ObservationSimulator<6,double,double>>,
               tom::ObservationSimulatorBase<double,double>>(m, "ObservationSimulator_6");



}

}// namespace tudatpy
