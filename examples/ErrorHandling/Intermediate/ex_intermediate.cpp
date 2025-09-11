//==============================================================================
// Name        : ex_intermediate.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.1
// Description : Intermediate demo for ErrorHandling: traits + multi-step
//               pipeline converting exceptions into status (no make failure).
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
#include <VoronoiMeshMaker/IO/IOHelpers.hpp>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>

// Bring only what we need into local scope.
using vmm::io::HeaderPrinter;
using vmm::io::PrintLine;
using vmm::io::CommentPrinter;
using vmm::error::VMMException;


using namespace std::literals::string_view_literals;

//------------------------------------------------------------------------------
// IntermediateError: enum for this example's domain.
//------------------------------------------------------------------------------
enum class IntermediateError {
    ConfigMissing = 1,
    BadFormat     = 2,
    Overflow      = 3
};

//------------------------------------------------------------------------------
// ErrorTraits specialization for IntermediateError (required by VMMException).
//------------------------------------------------------------------------------
namespace vmm::error {

template <>
struct ErrorTraits<IntermediateError> {
    static constexpr uint16_t domain_id() noexcept { return 0xD100; }

    static constexpr std::string_view domain_name() noexcept {
        return "IntermediateError";
    }

    static constexpr std::string_view key(IntermediateError e) noexcept {
        switch (e) {
            case IntermediateError::ConfigMissing: return "ConfigMissing";
            case IntermediateError::BadFormat:     return "BadFormat";
            case IntermediateError::Overflow:      return "Overflow";
        }
        return "Unknown";
    }

    static constexpr std::string_view enUS(IntermediateError e) noexcept {
        switch (e) {
            case IntermediateError::ConfigMissing:
                return "Configuration file not found.";
            case IntermediateError::BadFormat:
                return "Input data has invalid format.";
            case IntermediateError::Overflow:
                return "Computation overflow detected.";
        }
        return "Unknown error.";
    }

    static constexpr std::string_view ptBR(IntermediateError e) noexcept {
        switch (e) {
            case IntermediateError::ConfigMissing:
                return "Arquivo de configuracao nao encontrado.";
            case IntermediateError::BadFormat:
                return "Formato de dados de entrada invalido.";
            case IntermediateError::Overflow:
                return "Overflow detectado no calculo.";
        }
        return "Erro desconhecido.";
    }

    static constexpr Severity default_severity(IntermediateError e) noexcept {
        switch (e) {
            case IntermediateError::ConfigMissing: return Severity::Warning;
            case IntermediateError::BadFormat:     return Severity::Error;
            case IntermediateError::Overflow:      return Severity::Error;
        }
        return Severity::Error;
    }
};

} // namespace vmm::error

//------------------------------------------------------------------------------
// Domain-like steps that may throw.
//------------------------------------------------------------------------------
static void parse_config(std::string_view path) {
    (void)path; // simulate a missing config to demonstrate the flow
    throw VMMException(
        IntermediateError::ConfigMissing,
        {
            {"en"sv, "Missing config: expected file at './config.yml'."sv},
            {"pt"sv, "Falta configuracao: esperado './config.yml'."sv}
        }
    );
}

static void load_data(std::string_view file) {
    (void)file; // simulate a bad CSV format
    throw VMMException(
        IntermediateError::BadFormat,
        {
            {"en"sv, "CSV parse error at line 12, column 3."sv},
            {"pt"sv, "Erro ao ler CSV na linha 12, coluna 3."sv}
        }
    );
}

static void compute_core() {
    // simulate a numeric overflow
    throw VMMException(
        IntermediateError::Overflow,
        {
            {"en"sv, "Value exceeded numeric limits during iteration."sv},
            {"pt"sv, "Valor excedeu limites numericos na iteracao."sv}
        }
    );
}

//------------------------------------------------------------------------------
// Helper to run a step and convert exception to boolean status.
//------------------------------------------------------------------------------
template <class Fn>
bool run_step(const char* title, Fn&& fn) {
    std::cout << PrintLine();
    std::cout << CommentPrinter(std::string("Running step: ") + title);
    std::cout << PrintLine() << "\n";

    try {
        std::forward<Fn>(fn)();
        std::cout << "[OK] " << title << "\n\n";
        return true;
    } catch (const VMMException& ex) {
        std::cerr << "[VMMException] " << ex.what() << "\n\n";
        return false;
    } catch (const std::exception& ex) {
        std::cerr << "[std::exception] " << ex.what() << "\n\n";
        return false;
    }
}

int main() {
    // Header with default license block printed by your IO helpers.
    HeaderPrinter hp_default(
        "ex_intermediate.cpp",
        "João Flávio Vasconcellos",
        "1.0.1",
        "Intermediate demo for ErrorHandling: traits + multi-step pipeline"
    );
    std::cout << hp_default << "\n";

    int ok = 0;
    int fail = 0;

    // Run steps; each may fail, but we keep going to show partial progress.
    run_step("parse_config", [] { parse_config("./config.yml"); })
        ? ++ok : ++fail;

    run_step("load_data", [] { load_data("./data.csv"); })
        ? ++ok : ++fail;

    run_step("compute_core", [] { compute_core(); })
        ? ++ok : ++fail;

    std::cout << PrintLine();
    std::cout << CommentPrinter("Summary");
    std::cout << PrintLine() << "\n";
    std::cout << "Succeeded steps: " << ok << "\n";
    std::cout << "Failed steps   : " << fail << "\n\n";

    std::cout << PrintLine();
    std::cout << CommentPrinter(
        "Program ex_intermediate.cpp finished (no make fail)"
    );
    std::cout << PrintLine() << "\n\n";

    // Always succeed so `make run_ex_intermediate` is green.
    return EXIT_SUCCESS;
}
