/*!
 *  Copyright (c) 2015 by Contributors
 * \file ndarray_op.h
 * \brief the real execution functions of ndarray operations
 */
#ifndef MXNET_NDARRAY_NDARRAY_FUNCTION_H_
#define MXNET_NDARRAY_NDARRAY_FUNCTION_H_
#include <dmlc/logging.h>
#include <mshadow/tensor.h>
#include <mxnet/base.h>
#include <mxnet/resource.h>

namespace mxnet {
/*! \brief namespace to support all possible Ndarray operator */
namespace ndarray {
struct BinaryBase {
  inline static TShape GetShape(const TShape &lshape, const TShape &rshape) {
    CHECK(lshape == rshape) << "operands shape mismatch";
    CHECK(lshape.ndim() != 0) << "source operand have zero dimension shape";
    return lshape;
  }
};
// operators
struct Plus : public BinaryBase {
  typedef mshadow::op::plus mshadow_op;
};

struct Minus : public BinaryBase {
  typedef mshadow::op::minus mshadow_op;
};

struct Mul : public BinaryBase {
  typedef mshadow::op::mul mshadow_op;
};

struct Div : public BinaryBase {
  typedef mshadow::op::div mshadow_op;
};

struct Clip : public BinaryBase {
  struct mshadow_op {
    MSHADOW_XINLINE static real_t Map(real_t a, real_t b) {
      if (isnan(a)) return 0.0f;
      if (a < -b) return -b;
      if (a > b) return b;
      return a;
    }
  };
};
// type holder for random number generators
struct UniformDistribution {};

struct GaussianDistribution {};


template<typename Device, typename OP>
void Eval(const TBlob &lhs, const TBlob &rhs, TBlob *ret, RunContext ctx);

template<typename Device, typename OP, bool reverse>
void Eval(const TBlob &lhs, const real_t &rhs, TBlob *ret, RunContext ctx);

template<typename Device>
void Eval(const real_t &rhs, TBlob *ret, RunContext ctx);

template<typename Device, typename Distribution>
void EvalRandom(const real_t &a,
                const real_t &b,
                const Resource &resource,
                TBlob *ret,  RunContext ctx);

// copy function when only cpu is involved
template<typename DeviceFrom, typename DeviceTo>
void Copy(const TBlob &from, TBlob *to,
          Context from_ctx, Context to_ctx,
          RunContext ctx);

}  // namespace ndarray
}  // namespace mxnet
#endif  // MXNET_NDARRAY_NDARRAY_FUNCTION_H_
