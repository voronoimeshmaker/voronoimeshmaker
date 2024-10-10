cmake_minimum_required(VERSION 3.10)

# Nome fixo para o executável
set(example_name "Ex_Shape2D")

# Define o caminho para os arquivos fonte e de cabeçalhos
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Adiciona o arquivo Ex_Shape2D.cpp do diretório atual
set(ADDITIONAL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Ex_Shape2D.cpp)

# Verifica se o arquivo Ex_Shape2D.cpp realmente existe antes de prosseguir
if(NOT EXISTS ${ADDITIONAL_SOURCE})
    message(FATAL_ERROR "The source file ${ADDITIONAL_SOURCE} was not found. Please ensure the file exists.")
endif()

# Cria o executável a partir dos arquivos fonte encontrados e o arquivo adicional
add_executable(${example_name} ${ADDITIONAL_SOURCE})

# Linka o executável com a biblioteca VoronoiMeshMaker e suas dependências
target_link_libraries(${example_name} PRIVATE 
    VoronoiMeshMaker 
    gmp 
    mpfr 
    TBB::tbb
)

# Inclui os diretórios de cabeçalhos
target_include_directories(${example_name} PRIVATE 
    ${INCLUDE_DIR} 
    ${CMAKE_SOURCE_DIR}/Lib/include 
    ${HDF5_INCLUDE_DIRS}
)

# Define opções de compilação para otimização
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNU)
    target_compile_options(${example_name} PRIVATE -O3 -march=native -flto)
elseif(MSVC)
    target_compile_options(${example_name} PRIVATE /O2 /GL /DNDEBUG)
endif()

# Define o diretório de saída do executável como o diretório do respectivo arquivo fonte
set_target_properties(${example_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Adiciona o exemplo ao alvo run_examples, caso ele exista
if(TARGET run_examples)
    add_dependencies(run_examples ${example_name})
    add_custom_command(
        TARGET ${example_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Executable ${example_name} successfully built!"
        COMMAND ${CMAKE_COMMAND} -E echo "Running example ${example_name}..."
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${example_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running example ${example_name}..."
    )
endif()

# Mensagens de depuração (opcional)
message(STATUS "SOURCES: ${ADDITIONAL_SOURCE}")
