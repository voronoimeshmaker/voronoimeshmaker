// -----------------------------------------------------------------------------
// ut_traits.compile.cpp
// Compile-time checks for ErrorTraits/CoreErr mapping (no runtime).
// Build tip: add this TU to an OBJECT library or a separate target that is
//            only compiled (no need to link or run).
// Comments/messages in English.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  include VoronoiMeshMaker
// -----------------------------------------------------------------------------
#include <VoronoiMeshMaker/ErrorHandling/CoreErrors.h>
#include <VoronoiMeshMaker/ErrorHandling/ErrorTraits.h>

namespace ve = vmm::error;

// -----------------------------------------------------------------------------
//  compile-time assertions
// -----------------------------------------------------------------------------

// --- Basic enum sanity -------------------------------------------------------
static_assert(std::is_enum_v<ve::CoreErr>, "CoreErr must be an enum");

// --- ErrorEnum concept participation / API surface ---------------------------
// Ensure error_code(CoreErr) is well-formed.
static_assert(requires { ve::error_code(ve::CoreErr::InvalidArgument); },
              "error_code(CoreErr) must be well-formed");

// error_code(CoreErr) should produce a 32-bit unsigned integral.
static_assert(std::same_as<
                  decltype(ve::error_code(ve::CoreErr::InvalidArgument)),
                  std::uint32_t>,
              "error_code(CoreErr) must return std::uint32_t");

// ErrorTraits for CoreErr must expose text and severity APIs with the
// expected signatures.
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

// --- Optional: compile-time probes (non-binding) -----------------------------
// These only check that expressions are well-formed.
constexpr bool _vmm_traits_compile_sentinel = []{
    (void)ve::ErrorTraits<E>::enUS(E::InvalidArgument);
    (void)ve::ErrorTraits<E>::ptBR(E::InvalidArgument);
    (void)ve::ErrorTraits<E>::default_severity(E::InvalidArgument);
    (void)ve::error_code(E::InvalidArgument);
    return true;
}();

static_assert(_vmm_traits_compile_sentinel, "Compile-time probes must pass");
