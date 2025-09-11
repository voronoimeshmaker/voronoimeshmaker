//==============================================================================
// Name        : ut_Traits_Compile.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.3
// Description : Compile-time checks for ErrorTraits/CoreErr mapping (no runtime).
//               Tip: add this TU to an OBJECT library or a target that is only
//               compiled (no need to link or run).
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//==============================================================================

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>

namespace ve = vmm::error;

//------------------------------------------------------------------------------
// Compile-time assertions
//------------------------------------------------------------------------------

// --- Basic enum sanity --------------------------------------------------------
static_assert(std::is_enum_v<ve::CoreErr>, "CoreErr must be an enum");

// --- ErrorEnum concept participation / API surface ----------------------------
// Ensure error_code(CoreErr) is well-formed.
static_assert(requires { ve::error_code(ve::CoreErr::InvalidArgument); },
              "error_code(CoreErr) must be well-formed");

// error_code(CoreErr) should produce a 32-bit unsigned integral.
static_assert(std::same_as<
                  decltype(ve::error_code(ve::CoreErr::InvalidArgument)),
                  std::uint32_t>,
              "error_code(CoreErr) must return std::uint32_t");

// ErrorTraits for CoreErr must expose text and severity APIs
// with the expected signatures.
using E = ve::CoreErr;

static_assert(std::same_as<
                  decltype(ve::ErrorTraits<E>::enUS(E{})),
                  std::string_view>,
              "ErrorTraits<CoreErr>::enUS must return std::string_view");

static_assert(std::same_as<
                  decltype(ve::ErrorTraits<E>::ptBR(E{})),
                  std::string_view>,
              "ErrorTraits<CoreErr>::ptBR must return std::string_view");

static_assert(std::same_as<
                  decltype(ve::ErrorTraits<E>::default_severity(E{})),
                  ve::Severity>,
              "ErrorTraits<CoreErr>::default_severity must return Severity");

// domain_id must exist and be an unsigned integral (typically uint16_t).
static_assert(std::unsigned_integral<decltype(ve::ErrorTraits<E>::domain_id())>,
              "ErrorTraits<CoreErr>::domain_id must be an unsigned integral");

// --- Optional: compile-time probes (non-binding) ------------------------------
// These only check that expressions are well-formed.
constexpr bool _vmm_traits_compile_sentinel = [] {
    (void)ve::ErrorTraits<E>::enUS(E::InvalidArgument);
    (void)ve::ErrorTraits<E>::ptBR(E::InvalidArgument);
    (void)ve::ErrorTraits<E>::default_severity(E::InvalidArgument);
    (void)ve::error_code(E::InvalidArgument);
    return true;
}();

static_assert(_vmm_traits_compile_sentinel,
              "Compile-time probes must pass");
