// -----------------------------------------------------------------------------
// ut_IoHelpers.cpp
// Unit tests for PrintLine, HeaderPrinter, and CommentPrinter (GoogleTest).
// Comments/messages in English. Lines kept <= 80 columns.
// -----------------------------------------------------------------------------
//
// Build (example):
//   g++ -std=c++20 -O2 ut_IoHelpers.cpp \
//       -I<path-to-includes> -lgtest -lgtest_main -pthread -o ut_iohelpers
//
// -----------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <sstream>
#include <string>


#include <VoronoiMeshMaker/IO/IOHelpers.hpp>

namespace {

using vmm::io::PrintLine;
using vmm::io::HeaderPrinter;
using vmm::io::CommentPrinter;

// Helper: returns the first line of a multi-line string (without '\n')
std::string FirstLine(const std::string& s) {
    const auto pos = s.find('\n');
    return (pos == std::string::npos) ? s : s.substr(0, pos);
}

}  // namespace

// ----- PrintLine tests -------------------------------------------------------

TEST(PrintLineTest, PrintsDefaultLength) {
    std::ostringstream os;
    PrintLine pl;  // default line size is 80 (as defined in the header)
    os << pl;

    const std::string out = os.str();
    // Expect 80 '=' plus a trailing newline
    ASSERT_EQ(out.size(), 80u + 1u);
    EXPECT_EQ(out.back(), '\n');

    const std::string line = FirstLine(out);
    EXPECT_EQ(line.size(), 80u);
    EXPECT_TRUE(std::all_of(line.begin() + 3, line.end(),
                            [](char c) { return c == '='; }));
    EXPECT_TRUE(std::all_of(line.begin(), line.begin() + 1,
                            [](char c) { return c == '/'; }));
    EXPECT_TRUE(std::all_of(line.begin() + 2, line.begin() + 2,
                            [](char c) { return c == ' '; }));
}

TEST(PrintLineTest, ResizesFluently) {
    std::ostringstream os;
    PrintLine pl(10);
    os << pl;

    std::string out = os.str();
    EXPECT_EQ(FirstLine(out).size(), 10u);

    pl.size(25);
    os.str(std::string());
    os.clear();
    os << pl;

    out = os.str();
    EXPECT_EQ(FirstLine(out).size(), 25u);
}

// ----- HeaderPrinter tests ---------------------------------------------------

TEST(HeaderPrinterTest, PrintsHeaderWithDefaultLicense) {
    std::ostringstream os;
    HeaderPrinter hp(
        "IOHelpers.hpp",
        "VoronoiMeshMaker Team",
        "1.0.0",
        "Helper classes for formatted output"
    );
    os << hp;
    const std::string out = os.str();

    // Top border should be '//' + (lineSize-2) '='; default lineSize = 80
    // So first line is 2 + 78 = 80 characters total.
    const std::string first = FirstLine(out);
    ASSERT_GE(first.size(), 2u);
    EXPECT_EQ(first.substr(0, 2), "//");
    EXPECT_EQ(first.size(), 80u);
    for (size_t i = 2; i < first.size(); ++i) {
        EXPECT_EQ(first[i], '=');
    }

    // Must contain the info labels (Portuguese labels as in header).
    EXPECT_NE(out.find("Nome        : IOHelpers.hpp"), std::string::npos);
    EXPECT_NE(out.find("Autor       : VoronoiMeshMaker Team"), std::string::npos);
    EXPECT_NE(out.find("Versao      : 1.0.0"), std::string::npos);
    EXPECT_NE(out.find("Descricao   :"), std::string::npos);

    // Default license includes "Licenca Publica Geral GNU"
    EXPECT_NE(out.find("Licenca Publica Geral GNU"), std::string::npos);
}

TEST(HeaderPrinterTest, SupportsCustomLicenseAndLineSize) {
    std::ostringstream os;
    const std::string custom =
        "Line A\n"
        "Line B: custom license block";

    HeaderPrinter hp(
        "MyTool.cpp",
        "User",
        "0.2.0",
        "Demo header with custom license",
        custom,
        60
    );
    os << hp;
    const std::string out = os.str();

    // Border size should be 60 total chars in first line
    const std::string first = FirstLine(out);
    ASSERT_GE(first.size(), 2u);
    EXPECT_EQ(first.substr(0, 2), "//");
    EXPECT_EQ(first.size(), 60u);

    // Custom license lines must appear
    EXPECT_NE(out.find("Line A"), std::string::npos);
    EXPECT_NE(out.find("Line B: custom license block"), std::string::npos);
}

TEST(HeaderPrinterTest, FluentSettersWork) {
    std::ostringstream os;

    HeaderPrinter hp(
        "Title",
        "Author",
        "v",
        "Desc"
    );

    // Switch to custom license and different width, then back to default.
    hp.lineSize(72).licenseText("X\nY");
    os << hp;
    auto out = os.str();
    EXPECT_NE(out.find("X"), std::string::npos);
    EXPECT_NE(out.find("Y"), std::string::npos);

    os.str(std::string());
    os.clear();

    hp.licenseText("");  // empty -> default license
    os << hp;
    out = os.str();
    EXPECT_NE(out.find("Licenca Publica Geral GNU"), std::string::npos);
}

// ----- CommentPrinter tests --------------------------------------------------

TEST(CommentPrinterTest, PrintsEachLineWithPrefix) {
    std::ostringstream os;
    CommentPrinter cp("A\nB\nC");
    os << cp;

    const std::string out = os.str();
    // Expect three lines, each starting with "// "
    size_t start = 0;
    int count = 0;
    while (true) {
        const size_t eol = out.find('\n', start);
        if (eol == std::string::npos) break;
        const std::string line = out.substr(start, eol - start);
        EXPECT_EQ(line.rfind("// ", 0), 0u);
        start = eol + 1;
        ++count;
    }
    EXPECT_EQ(count, 3);
}

TEST(CommentPrinterTest, SetterUpdatesText) {
    std::ostringstream os;
    CommentPrinter cp("First line");
    cp.text("Changed");
    os << cp;

    const std::string out = os.str();
    EXPECT_NE(out.find("// Changed"), std::string::npos);
}
