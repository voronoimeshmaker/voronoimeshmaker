//==============================================================================
// Nome        : main.cpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Programa de teste da biblioteca TAO do Petsc
//
// Este programa é um software livre: você pode redistribuí-lo e/ou modificá-lo
// sob os termos da Licença Pública Geral GNU, conforme publicada pela
// Free Software Foundation, na versão 3 da licença.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO. Veja a
// Licença Pública Geral GNU para mais detalhes.
//==============================================================================


#include <iostream>
#include <string>

// Adjust path if your include tree differs:
#include <VoronoiMeshMaker/IO/IOHelpers.hpp>
    using vmm::io::PrintLine;
    using vmm::io::HeaderPrinter;
    using vmm::io::CommentPrinter;

int main() {

    // ---- HeaderPrinter (default license text) -------------------------------
//    std::cout << "\nHeaderPrinter (default license):\n";
    HeaderPrinter hp_default(
        "IOHelpers.hpp",
        "VoronoiMeshMaker Team",
        "1.0.0",
        "Helper classes for formatted output (line, header, comments)"
    );
    std::cout << hp_default;


    // ---- PrintLine ----------------------------------------------------------
    std::cout << "PrintLine demo:\n";
    PrintLine pl_default;          // default line size (80)
    std::cout << pl_default;

    PrintLine pl_custom(50);       // custom size
    std::cout << pl_custom;

    pl_custom.size(30);            // fluent-style size change
    std::cout << pl_custom;


    // ---- HeaderPrinter (custom license + different line size) ---------------
    std::cout << "\nHeaderPrinter (custom license, 60 cols):\n";
    const std::string custom_license =
        "This software is provided \"as is\" without any warranty.\n"
        "Redistribution and use are permitted under the project terms.";

    HeaderPrinter hp_custom(
        "MyTool.cpp",
        "Your Name",
        "0.1.0",
        "Small demo using IO helpers",
        custom_license,
        60
    );
    std::cout << hp_custom;

    // Change line size and license on the fly (fluent API):
    hp_custom.lineSize(72).licenseText("");  // back to default license
    std::cout << hp_custom;

    // ---- CommentPrinter -----------------------------------------------------
    std::cout << "\nCommentPrinter demo:\n";
    CommentPrinter cp(
        "This is a multi-line comment block.\n"
        "Each line is prefixed with \"// \".\n"
        "Useful for documenting code sections."
    );
    std::cout << cp;

    // Update text using the fluent setter:
    cp.text("Single line after update.");
    std::cout << cp;

    return 0;
}
