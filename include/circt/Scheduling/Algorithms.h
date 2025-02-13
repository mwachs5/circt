//===- Algorithms.h - Library of scheduling algorithms ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines a library of scheduling algorithms.
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_SCHEDULING_ALGORITHMS_H
#define CIRCT_SCHEDULING_ALGORITHMS_H

#include "circt/Scheduling/Problems.h"

namespace circt {
namespace scheduling {

/// This is a simple list scheduler for solving the basic scheduling problem.
/// Its objective is to assign each operation its earliest possible start time,
/// or in other words, to schedule each operation as soon as possible (hence the
/// name). Fails if the dependence graph contains cycles.
LogicalResult scheduleASAP(Problem &prob);

/// This scheduler solves the resource-free cyclic problem using linear
/// programming and a handwritten implementation of the simplex algorithm. Its
/// objectives are to determine the smallest feasible initiation interval, and
/// to minimize the start time of the given \p lastOp. Fails if the dependence
/// graph contains cycles that do not include at least one edge with a non-zero
/// distance.
LogicalResult scheduleSimplex(CyclicProblem &prob, Operation *lastOp);

} // namespace scheduling
} // namespace circt

#endif // CIRCT_SCHEDULING_ALGORITHMS_H
