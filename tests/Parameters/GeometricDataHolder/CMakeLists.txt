cmake_minimum_required(VERSION 3.10)

# Nome do projeto para o teste específico
project(GeometricDataHolderTest)

# Arquivo de teste
set(TEST_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/GT_GeometricDataHolder.cpp)

# Criação do executável de teste
add_executable(${PROJECT_NAME} ${TEST_SOURCE})

# Linka o executável com Google Test, VoronoiMeshMaker e outras dependências necessárias
target_link_libraries(${PROJECT_NAME} PRIVATE 
    GTest::GTest 
    GTest::Main 
    VoronoiMeshMaker 
    gmp 
    mpfr 
    TBB::tbb
)

# Inclui diretórios de cabeçalho
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Lib/include 
    ${CMAKE_SOURCE_DIR}/tests/Parameters/GeometricDataHolder/include
    ${HDF5_INCLUDE_DIRS}
)

# Define opções de compilação para otimização (opcional)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNU)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native -flto)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /O2 /GL /DNDEBUG)
endif()

# Define o diretório de saída do executável como o diretório atual
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Adiciona o teste ao CTest para ser executado automaticamente
add_test(NAME ${PROJECT_NAME} COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME})

# Mensagens de depuração (opcional)
message(STATUS "Adding test executable for GeometricDataHolder: ${PROJECT_NAME}")

# Adiciona um comando personalizado para rodar o executável manualmente após a compilação (opcional)
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Executable ${PROJECT_NAME} successfully built!"
    COMMAND ${CMAKE_COMMAND} -E echo "Running test executable..."
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running test executable ${PROJECT_NAME}..."
)
