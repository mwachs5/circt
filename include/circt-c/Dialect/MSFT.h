//===-- circt-c/Dialect/MSFT.h - C API for MSFT dialect -----------*- C -*-===//
//
// This header declares the C interface for registering and accessing the
// MSFT dialect. A dialect should be registered with a context to make it
// available to users of the context. These users must load the dialect
// before using any of its attributes, operations or types. Parser and pass
// manager can load registered dialects automatically.
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_C_DIALECT_MSFT_H
#define CIRCT_C_DIALECT_MSFT_H

#include "circt/Dialect/MSFT/MSFTDialect.h"
#include "mlir-c/IR.h"
#include "mlir-c/Pass.h"
#include "mlir-c/Registration.h"

#ifdef __cplusplus
extern "C" {
#endif

MLIR_DECLARE_CAPI_DIALECT_REGISTRATION(MSFT, msft);

MLIR_CAPI_EXPORTED void mlirMSFTRegisterPasses();

/// Emits tcl for the specified module using the provided callback and user
/// data
MLIR_CAPI_EXPORTED MlirLogicalResult mlirMSFTExportTcl(MlirModule,
                                                       MlirStringCallback,
                                                       void *userData);

/// This callback constructs a replacement for the operation argument and
/// returns it.
typedef struct {
  MlirOperation (*callback)(MlirOperation, void *userData);
  void *userData;
} mlirMSFTGeneratorCallback;

/// Register a generator callback (function pointer, user data pointer).
MLIR_CAPI_EXPORTED void mlirMSFTRegisterGenerator(MlirContext,
                                                  const char *opName,
                                                  const char *generatorName,
                                                  mlirMSFTGeneratorCallback cb,
                                                  MlirAttribute parameters);

/// Add a physical location attribute with the given entity name, device type, x
/// and y coordinates, and number.
MLIR_CAPI_EXPORTED void mlirMSFTAddPhysLocationAttr(MlirOperation op,
                                                    const char *entityName,
                                                    DeviceType type, long x,
                                                    long y, long num);

#ifdef __cplusplus
}
#endif

#endif // CIRCT_C_DIALECT_MSFT_H
