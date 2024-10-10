cmake_minimum_required(VERSION 3.10)

# Define o nome do projeto
project(AllTests)

# Inclui Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Função para adicionar todos os subdiretórios recursivamente
function(add_subdirectories_recursive dir)
    file(GLOB SUBDIRS RELATIVE ${dir} ${dir}/*)
    foreach(subdir ${SUBDIRS})
        if(IS_DIRECTORY ${dir}/${subdir})
            add_subdirectory(${dir}/${subdir})
        endif()
    endforeach()
endfunction()

# Adiciona todos os subdiretórios recursivamente
add_subdirectories_recursive(${CMAKE_CURRENT_SOURCE_DIR})

# Cria um alvo para rodar todos os testes (opcional)
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests..."
)

# Mensagem de status
message(STATUS "Adding all subdirectories for tests in ${CMAKE_CURRENT_SOURCE_DIR}")
