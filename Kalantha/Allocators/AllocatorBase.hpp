/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Errors.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Core/Result.hpp>
BEGIN_NAMESPACE_KTA_

class AllocatorBase {
public:
  template<typename T>
  using __Result = kta::Result<T, Error>;

  template<typename T>
  FORCEINLINE_ auto deallocate(this auto&& self, T* ptr) -> __Result<void> {
    return self.template deallocate_<T>(ptr);
  }

  template<typename T, typename ...Args>
  FORCEINLINE_ auto allocate(this auto&& self, Args&&... args) -> T* {
    return self.template allocate_<T>(kta::forward<Args>(args)...);
  }

  FORCEINLINE_ auto remaining(this auto&& self) -> usize {
    return self.remaining_();
  }

  AllocatorBase() = default;
 ~AllocatorBase() = default;
};

END_NAMESPACE_KTA_
