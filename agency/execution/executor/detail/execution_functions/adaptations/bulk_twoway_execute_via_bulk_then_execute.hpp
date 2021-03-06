// Copyright (c) 2018, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <agency/detail/config.hpp>
#include <agency/detail/requires.hpp>
#include <agency/detail/type_traits.hpp>
#include <agency/execution/executor/executor_traits/executor_execution_depth.hpp>
#include <agency/execution/executor/executor_traits/executor_future.hpp>
#include <agency/execution/executor/executor_traits/executor_shape.hpp>
#include <agency/execution/executor/executor_traits/detail/is_bulk_then_executor.hpp>
#include <agency/future/future_traits.hpp>


namespace agency
{
namespace detail
{


__agency_exec_check_disable__
template<class BulkThenExecutor, class Function, class ResultFactory, class... Factories,
         __AGENCY_REQUIRES(is_bulk_then_executor<BulkThenExecutor>::value),
         __AGENCY_REQUIRES(executor_execution_depth<BulkThenExecutor>::value == sizeof...(Factories))
        >
__AGENCY_ANNOTATION
executor_future_t<BulkThenExecutor, result_of_t<ResultFactory()>>
  bulk_twoway_execute_via_bulk_then_execute(const BulkThenExecutor& ex, Function f, executor_shape_t<BulkThenExecutor> shape, ResultFactory result_factory, Factories... shared_factories)
{
  using void_future_type = executor_future_t<BulkThenExecutor, void>;

  // XXX we might want to actually allow the executor to participate here
  auto predecessor = future_traits<void_future_type>::make_ready();

  return ex.bulk_then_execute(f, shape, predecessor, result_factory, shared_factories...);
}


} // end detail
} // end agency

