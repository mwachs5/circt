//===- MSFTModule.cpp - MSFT API pybind module ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DialectModules.h"

#include "circt-c/Dialect/MSFT.h"
#include "circt/Dialect/MSFT/MSFTAttributes.h"
#include "circt/Support/LLVM.h"

#include "mlir/Bindings/Python/PybindAdaptors.h"
#include "mlir/CAPI/IR.h"
#include "mlir/CAPI/Support.h"

#include "PybindUtils.h"
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

using namespace circt;
using namespace circt::msft;

static MlirOperation callPyFunc(MlirOperation op, void *userData) {
  py::gil_scoped_acquire gil;
  auto replacement = (*(py::function *)userData)(op);
  return replacement.cast<MlirOperation>();
}

static void registerGenerator(MlirContext ctxt, std::string opName,
                              std::string generatorName, py::function cb,
                              MlirAttribute parameters) {
  // Since we don't have an 'unregister' call, just allocate in forget about it.
  py::function *cbPtr = new py::function(cb);
  mlirMSFTRegisterGenerator(ctxt, opName.c_str(), generatorName.c_str(),
                            mlirMSFTGeneratorCallback{&callPyFunc, cbPtr},
                            parameters);
}

/// Populate the msft python module.
void circt::python::populateDialectMSFTSubmodule(py::module &m) {
  mlirMSFTRegisterPasses();

  m.doc() = "MSFT dialect Python native extension";

  m.def("locate", &mlirMSFTAddPhysLocationAttr,
        "Attach a physical location to an op's entity.",
        py::arg("op_to_locate"), py::arg("entity_within"), py::arg("devtype"),
        py::arg("x"), py::arg("y"), py::arg("num"));

  py::enum_<DeviceType>(m, "DeviceType")
      .value("M20K", DeviceType::M20K)
      .value("DSP", DeviceType::DSP)
      .export_values();

  m.def("export_tcl", [](MlirModule mod, py::object fileObject) {
    circt::python::PyFileAccumulator accum(fileObject, false);
    py::gil_scoped_release();
    mlirMSFTExportTcl(mod, accum.getCallback(), accum.getUserData());
  });

  m.def("register_generator", &::registerGenerator,
        "Register a generator for a design module");
}
