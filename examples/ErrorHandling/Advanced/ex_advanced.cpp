//==============================================================================
// Name        : ex_advanced.cpp
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.0.0
// Description : Advanced demo for ErrorHandling: parallel pipeline with
//               jthreads, per-thread breadcrumbs, and single top-level report.
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
//      C++ standard library includes
//==============================================================================
#include <latch>         // std::latch
#include <mutex>         // std::mutex, std::scoped_lock
#include <thread>        // std::jthread
#include <vector>        // std::vector 

//==============================================================================
//      VoronoiMeshMaker includes
//==============================================================================
#include <VoronoiMeshMaker/IO/IOHelpers.hpp>
#include <VoronoiMeshMaker/ErrorHandling/VMMException.h>

using vmm::io::HeaderPrinter;
using vmm::io::PrintLine;
using vmm::io::CommentPrinter;
using vmm::error::VMMException;
using vmm::error::Severity;

using namespace std::literals::string_view_literals;

//==============================================================================
//      Simple per-thread breadcrumbs (demo-only, local to this example)
//==============================================================================
namespace {
thread_local std::vector<std::string> tl_trace;

struct TraceMark {
    explicit TraceMark(std::string msg) { tl_trace.emplace_back(std::move(msg)); }
};

static std::string trace_snapshot() {
    std::string out;
    for (std::size_t i = 0; i < tl_trace.size(); ++i) {
        if (i) out += " > ";
        out += tl_trace[i];
    }
    return out;
}

static void trace_clear() {
    tl_trace.clear();
}
} // namespace

//==============================================================================
//      Error domain for the advanced example
//==============================================================================
enum class AdvancedError {
    MissingConfig = 1,
    BadState      = 2,
    ExportDenied  = 3
};

namespace vmm::error {

template <>
struct ErrorTraits<AdvancedError> {
    static constexpr uint16_t domain_id() noexcept { return 0xD200; }

    static constexpr std::string_view domain_name() noexcept {
        return "AdvancedError";
    }

    static constexpr std::string_view key(AdvancedError e) noexcept {
        switch (e) {
            case AdvancedError::MissingConfig: return "MissingConfig";
            case AdvancedError::BadState:      return "BadState";
            case AdvancedError::ExportDenied:  return "ExportDenied";
        }
        return "Unknown";
    }

    static constexpr std::string_view enUS(AdvancedError e) noexcept {
        switch (e) {
            case AdvancedError::MissingConfig:
                return "Configuration file not found.";
            case AdvancedError::BadState:
                return "Mesh builder reached an invalid state.";
            case AdvancedError::ExportDenied:
                return "VTK export denied: insufficient permissions.";
        }
        return "Unknown error.";
    }

    static constexpr std::string_view ptBR(AdvancedError e) noexcept {
        switch (e) {
            case AdvancedError::MissingConfig:
                return "Arquivo de configuracao nao encontrado.";
            case AdvancedError::BadState:
                return "Construtor de malha atingiu estado invalido.";
            case AdvancedError::ExportDenied:
                return "Exportacao VTK negada: permissoes insuficientes.";
        }
        return "Erro desconhecido.";
    }

    static constexpr Severity default_severity(AdvancedError e) noexcept {
        switch (e) {
            case AdvancedError::MissingConfig: return Severity::Warning;
            case AdvancedError::BadState:      return Severity::Error;
            case AdvancedError::ExportDenied:  return Severity::Error;
        }
        return Severity::Error;
    }
};

} // namespace vmm::error

//==============================================================================
//      Domain-like tasks (some succeed, some throw)
//==============================================================================

static void task_read_config() {
    trace_clear();
    TraceMark t0("read_config:begin");
    TraceMark t1("read_config:open('./config.yml')");
    // Simulate success: do nothing else.
    TraceMark t2("read_config:end");
}

static void task_build_mesh() {
    trace_clear();
    TraceMark t0("build_mesh:begin");
    TraceMark t1("build_mesh:load_boundary");
    TraceMark t2("build_mesh:delaunay_init");
    // Simulate failure.
    throw VMMException(
        AdvancedError::BadState,
        {
            {"en"sv, "Delaunay builder failed: degenerate input."sv},
            {"pt"sv, "Falha no construtor Delaunay: entrada degenerada."sv}
        }
    );
}

static void task_export_vtk() {
    trace_clear();
    TraceMark t0("export_vtk:begin");
    TraceMark t1("export_vtk:mkdir('./out')");
    // Simulate failure (permission denied, for example).
    throw VMMException(
        AdvancedError::ExportDenied,
        {
            {"en"sv, "Cannot write './out/mesh.vtk'."sv},
            {"pt"sv, "Nao foi possivel escrever './out/mesh.vtk'."sv}
        }
    );
}

//==============================================================================
//      Result aggregation
//==============================================================================
struct TaskResult {
    std::string name;
    bool        ok;
    bool        is_vmm;
    std::string what;
    std::string trace;
};

int main() {
    // Header
    HeaderPrinter hp_default(
        "ex_advanced.cpp",
        "João Flávio Vasconcellos",
        "1.0.0",
        "Advanced demo: parallel tasks + breadcrumbs + single report"
    );
    std::cout << hp_default << "\n";

    std::cout << PrintLine();
    std::cout << CommentPrinter("Launching parallel tasks");
    std::cout << PrintLine() << "\n";

    std::vector<TaskResult> results;
    std::mutex results_mtx;

    // We will run 3 tasks in parallel.
    std::latch done(3);
    std::vector<std::jthread> threads;

    // Helper lambda to spawn a task that may throw and record status+trace.
    auto spawn = [&](const char* name, auto&& fn) {
        threads.emplace_back([&, name, fn]() {
            try {
                fn();
                TaskResult r{name, true, false, {}, trace_snapshot()};
                std::scoped_lock lk(results_mtx);
                results.emplace_back(std::move(r));
            } catch (const VMMException& ex) {
                TaskResult r{name, false, true, ex.what(), trace_snapshot()};
                std::scoped_lock lk(results_mtx);
                results.emplace_back(std::move(r));
            } catch (const std::exception& ex) {
                TaskResult r{name, false, false, ex.what(), trace_snapshot()};
                std::scoped_lock lk(results_mtx);
                results.emplace_back(std::move(r));
            } catch (...) {
                TaskResult r{name, false, false, "unknown exception",
                             trace_snapshot()};
                std::scoped_lock lk(results_mtx);
                results.emplace_back(std::move(r));
            }
            done.count_down();
        });
    };

    // Spawn all tasks.
    spawn("read_config",  [] { task_read_config();  });
    spawn("build_mesh",   [] { task_build_mesh();   });
    spawn("export_vtk",   [] { task_export_vtk();   });

    // Wait for all tasks to complete.
    done.wait();
    // jthreads join on destruction.

    // Single, top-level report (no nested prints).
    int ok = 0, fail = 0;
    for (const auto& r : results) (r.ok ? ok : fail)++;

    std::cout << PrintLine();
    std::cout << CommentPrinter("Summary");
    std::cout << PrintLine() << "\n";
    std::cout << "Succeeded tasks: " << ok << "\n";
    std::cout << "Failed tasks   : " << fail << "\n\n";

    if (fail) {
        std::cout << PrintLine();
        std::cout << CommentPrinter("Failures (message + breadcrumbs)");
        std::cout << PrintLine() << "\n";
        for (const auto& r : results) {
            if (!r.ok) {
                std::cerr << (r.is_vmm ? "[VMMException] " : "[exception] ")
                          << r.what << "\n";
                std::cerr << "Trace: " << r.trace << "\n\n";
            }
        }
    }

    std::cout << PrintLine();
    std::cout << CommentPrinter(
        "Program ex_advanced.cpp finished (no make fail)"
    );
    std::cout << PrintLine() << "\n\n";

    // Keep green for `make run_ex_advanced`.
    return EXIT_SUCCESS;
}
