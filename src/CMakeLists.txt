# ライブラリのビルド
add_library(hasami_lib cell.cpp board.cpp player.cpp)

# インクルードディレクトリの指定
target_include_directories(hasami_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
