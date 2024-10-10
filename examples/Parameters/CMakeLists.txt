cmake_minimum_required(VERSION 3.10)

# Define o nome do projeto
project(ParametersExamples)

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

# Mensagem indicando que não há programas diretamente nesta pasta
message(STATUS "No programs in the Parameters directory itself. Adding subdirectories if they exist.")
