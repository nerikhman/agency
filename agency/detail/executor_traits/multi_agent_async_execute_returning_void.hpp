#pragma once

#include <agency/detail/config.hpp>
#include <agency/future.hpp>
#include <agency/new_executor_traits.hpp>
#include <type_traits>

namespace agency
{
namespace detail
{
namespace new_executor_traits_detail
{


template<class Executor, class Function>
typename new_executor_traits<Executor>::template future<void>
  multi_agent_async_execute_returning_void(std::true_type, Executor& ex, Function f, typename new_executor_traits<Executor>::shape_type shape)
{
  return ex.async_execute(f, shape);
} // end multi_agent_async_execute_returning_void()


template<class Executor, class Function>
typename new_executor_traits<Executor>::template future<void>
  multi_agent_async_execute_returning_void(std::false_type, Executor& ex, Function f, typename new_executor_traits<Executor>::shape_type shape)
{
  auto ready = new_executor_traits<Executor>::template make_ready_future<void>(ex);
  return new_executor_traits<Executor>::then_execute(ex, f, shape, ready);
} // end multi_agent_async_returning_default_void()


template<class Executor, class Function>
struct has_multi_agent_async_execute_returning_void_impl
{
  template<class Executor1,
           class ReturnType = decltype(
             std::declval<Executor1>().async_execute(
               std::declval<Function>()
             )
           ),
           class = typename std::enable_if<
             std::is_void<ReturnType>::value
           >::type>
  static std::true_type test(int);

  template<class>
  static std::false_type test(...);

  using type = decltype(test<Executor>(0));
};

template<class Executor, class Function>
using has_multi_agent_async_execute_returning_void = typename has_multi_agent_async_execute_returning_void_impl<Executor,Function>::type;


} // end new_executor_traits_detail
} // end detail


template<class Executor>
  template<class Function,
           class Enable>
typename new_executor_traits<Executor>::template future<void>
  new_executor_traits<Executor>
    ::async_execute(typename new_executor_traits<Executor>::executor_type& ex,
                    Function f,
                    typename new_executor_traits<Executor>::shape_type shape)
{
  using check_for_member_function = detail::new_executor_traits_detail::has_multi_agent_async_execute_returning_void<
    Executor,
    Function
  >;

  return detail::new_executor_traits_detail::multi_agent_async_execute_returning_void(check_for_member_function(), ex, f, shape);
} // end new_executor_traits::async_execute()


} // end agency

