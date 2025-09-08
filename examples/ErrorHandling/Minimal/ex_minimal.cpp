// ============================================================================
// File: ex_minimal.cpp
// Purpose: Minimal end-to-end demo of VoronoiMeshMaker ErrorHandling.
// Build: add this file to your examples target and link as usual.
// Notes:
//   - Comments/messages in code are in English (project standard).
//   - Keep lines <= 80 cols when possible.
// ============================================================================

#include <iostream>
#include <string>
#include <string_view>
#include <utility>


// ErrorHandling headers
#include <VoronoiMeshMaker/ErrorHandling/ErrorHandling.h>
#include <VoronoiMeshMaker/IO/IOHelpers.hpp>

using namespace vmm::error;

// ---- Example API that uses exceptions --------------------------------------

int divide_or_throw(int a, int b) {
    // Precondition: b != 0
    VMM_REQUIRE(b != 0, CoreErr::InvalidArgument, {{"name", "b"}});
    return a / b;
}

// ---- Example API that uses StatusOr (no throw) ------------------------------

StatusOr<int> parse_positive_int(std::string_view s) {
    // Tiny parser: accept digits only, no sign.
    int value = 0;
    if (s.empty()) {
        auto msg = render(CoreErr::InvalidArgument, Language::EnUS,
                          {{"name", "empty string"}});
        return Status(error_code(CoreErr::InvalidArgument),
                      std::move(msg), Severity::Error);
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            auto msg = render(CoreErr::InvalidArgument, Language::EnUS,
                              {{"name", "non-digit char"}});
            return Status(error_code(CoreErr::InvalidArgument),
                          std::move(msg), Severity::Error);
        }
        value = value * 10 + (c - '0');
    }
    if (value <= 0) {
        auto msg = render(CoreErr::InvalidArgument, Language::EnUS,
                          {{"name", "non-positive value"}});
        return Status(error_code(CoreErr::InvalidArgument),
                      std::move(msg), Severity::Error);
    }
    return value;
}

// ---- Demo main --------------------------------------------------------------

int main() {



    // 1) Configure language to Portuguese (what() stays in English).
    ErrorConfig cfg;
    cfg.language = Language::PtBR;
    cfg.thread_buffer_cap = 8;
    Config::set(cfg);

    std::cout << "[1] Throw/try-catch demo\n";
    try {
        // This will throw: division by zero (b == 0).
        (void)divide_or_throw(10, 0);
    } catch (const VMMException& ex) {
        std::cout << "  what():   " << ex.what() << '\n';
        std::cout << "  message(): " << ex.message() << '\n';
        std::cout << "  code:     " << ex.code() << '\n';
        std::cout << "  key:      " << ex.key() << '\n';
        std::cout << "  where:    " << ex.location().file_name() << ':'
                  << ex.location().line() << " in "
                  << ex.location().function_name() << '\n';
    }

    // 2) Log a non-fatal issue (no throw) and flush.
    std::cout << "\n[2] Log/flush demo\n";
    {
        // Minimal manual record (avoids extra macros for clarity here).
        ErrorRecord rec;
        rec.code = error_code(CoreErr::NotImplemented);
        rec.severity = ErrorTraits<CoreErr>::default_severity(
            CoreErr::NotImplemented);
        rec.message = render(CoreErr::NotImplemented,
                             Config::get()->language, {});
        ErrorManager::log(std::move(rec));
    }
    auto recs = ErrorManager::flush();
    for (const auto& r : recs) {
        std::cout << "  logged: code=" << r.code << " sev="
                  << static_cast<int>(r.severity)
                  << " msg=\"" << r.message << "\"\n";
    }

    // 3) StatusOr (no exceptions).
    std::cout << "\n[3] StatusOr demo\n";
    auto okv = parse_positive_int("123");
    if (okv.ok()) {
        std::cout << "  parsed value = " << okv.value() << '\n';
    }
    auto errv = parse_positive_int("12x");
    if (!errv.ok()) {
        std::cout << "  status error: code=" << errv.status().code()
                  << " msg=\"" << errv.status().message() << "\"\n";
    }

    std::cout << "\nDone.\n";
    return 0;
}
