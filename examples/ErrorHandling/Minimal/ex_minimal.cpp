//==============================================================================
// Name        : ex_minimal.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.2
// Description : Minimal demo for ErrorHandling: throw/catch VMMException.
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

// Bring only what we need into the local scope.
using vmm::io::HeaderPrinter;
using vmm::io::PrintLine;
using vmm::io::CommentPrinter;
using vmm::error::VMMException;


// Enable "sv" literal for std::string_view ("text"sv).
using namespace std::literals::string_view_literals;

//------------------------------------------------------------------------------
// A simple local enum that satisfies the ErrorEnum concept via ErrorTraits.
// Replace with your official project enum if you already have one.
//------------------------------------------------------------------------------
enum class DemoError {
    InvalidInput = 1
};

//------------------------------------------------------------------------------
// ErrorTraits specialization for DemoError (required by VMMException concept).
//------------------------------------------------------------------------------
namespace vmm::error {

template <>
struct ErrorTraits<DemoError> {
    static constexpr uint16_t domain_id() noexcept { return 0xD001; }
    static constexpr std::string_view domain_name() noexcept {
        return "DemoError";
    }
    static constexpr std::string_view key(DemoError e) noexcept {
        switch (e) {
            case DemoError::InvalidInput: return "InvalidInput";
        }
        return "Unknown";
    }
    static constexpr std::string_view enUS(DemoError e) noexcept {
        switch (e) {
            case DemoError::InvalidInput:
                return "Invalid input for operation.";
        }
        return "Unknown error.";
    }
    static constexpr std::string_view ptBR(DemoError e) noexcept {
        switch (e) {
            case DemoError::InvalidInput:
                return "Entrada invalida para a operacao.";
        }
        return "Erro desconhecido.";
    }
    static constexpr Severity default_severity(DemoError) noexcept {
        return Severity::Error;
    }
};

} // namespace vmm::error

//------------------------------------------------------------------------------
// A tiny domain function that fails to demonstrate ErrorHandling.
//------------------------------------------------------------------------------
static void failing_operation() {
    // Pass enum + localized messages (EN/PT). The 3rd parameter
    // (std::source_location) is defaulted inside VMMException.
    throw VMMException(
        DemoError::InvalidInput,
        {
            {"en"sv,
             "Invalid input provided to failing_operation()."},
            {"pt"sv,
             "Entrada invalida fornecida para failing_operation()."}
        }
    );
}

int main() {
    //==============================================================================
    //      Default header/license printing
    //==============================================================================
    HeaderPrinter hp_default(
        "ex_minimal.cpp",
        "João Flávio Vasconcellos",
        "1.0.2",
        "Minimal demo for ErrorHandling: throw and catch VMMException"
    );
    std::cout << hp_default << "\n";

    std::cout << "\n\n" << PrintLine();
    std::cout << CommentPrinter("Starting minimal ErrorHandling demo");
    std::cout << PrintLine() << "\n\n";

    //==============================================================================
    //      Minimal try/catch usage
    //==============================================================================
    try {
        failing_operation();
        std::cout << "This line is never reached.\n";
    }
    catch (const VMMException& ex) {
        // Outermost catch: print user-facing message exactly once.
        std::cerr << "[VMMException] " << ex.what() << "\n";
        std::cerr << "Handling at the top level and exiting with failure.\n";

        std::cout << "\n\n" << PrintLine();
        std::cout << CommentPrinter(
            "Program ex_minimal.cpp finished with error"
        );
        std::cout << PrintLine() << "\n\n";
        return EXIT_FAILURE;
    }
    catch (const std::exception& ex) {
        // Fallback for unexpected std::exceptions.
        std::cerr << "[std::exception] " << ex.what() << "\n";

        std::cout << "\n\n" << PrintLine();
        std::cout << CommentPrinter(
            "Program ex_minimal.cpp finished with error"
        );
        std::cout << PrintLine() << "\n\n";
        return EXIT_FAILURE;
    }

    //==============================================================================
    //      End of program (success path)
    //==============================================================================
    std::cout << "\n\n" << PrintLine();
    std::cout << CommentPrinter(
        "Program ex_minimal.cpp finished successfully"
    );
    std::cout << PrintLine() << "\n\n";

    return EXIT_SUCCESS;
}
