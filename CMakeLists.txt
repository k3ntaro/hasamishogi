cmake_minimum_required(VERSION 3.10)
project(HasamiShogi)

# C++の標準を設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# インクルードディレクトリを設定
include_directories(${CMAKE_SOURCE_DIR}/include)

# srcディレクトリ内のCMakeLists.txtをサブディレクトリとして追加
add_subdirectory(src)

# main.cppのビルド設定
add_executable(hasamishogi main.cpp)
add_executable(hasamishogi_human_vs_ai vsAI.cpp)
add_executable(hasamishogi_ai_vs_ai AIvsAI.cpp)

# インクルードディレクトリをリンク
target_include_directories(hasamishogi PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hasamishogi_human_vs_ai PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(hasamishogi_ai_vs_ai PUBLIC ${CMAKE_SOURCE_DIR}/include)

# srcでビルドしたオブジェクトファイルをリンク
target_link_libraries(hasamishogi hasami_lib)
target_link_libraries(hasamishogi_human_vs_ai hasami_lib)
target_link_libraries(hasamishogi_ai_vs_ai hasami_lib)
