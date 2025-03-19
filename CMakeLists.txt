# CMakeLists.txt
# windows: cmake -B . -S .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=~/Code/vcpkg/scripts/buildsystems/vcpkg.cmake
# ubuntu : cmake -B . -S .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/home/ubuntu/vcpkg/scripts/buildsystems/vcpkg.cmake
# macos  : cmake -B . -S .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/Users/ubuntu/Code/vcpkg/scripts/buildsystems/vcpkg.cmake
# cmake --build . -j8
# cp libbeid.a ~/Code/SDK/lib/Release

cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0167 NEW)

project(beid)

# 1) COMPILER
#############

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
elseif(UNIX)
	add_compile_options(-Wall -Wextra)
endif()

# 2) PACKAGES
#############

find_package(Boost REQUIRED COMPONENTS system)

# 3) SOURCES
############

add_library(beid STATIC
	common/asn1.cpp
	common/BEIDCard.cpp
	common/CardFactory.cpp
	common/CardReader.cpp
	common/CertChainRequestHandler.cpp
	common/comm.cpp
	common/crypto.cpp
	common/hash.cpp
	common/IDRequestHandler.cpp
	common/log.cpp
	common/native.cpp
	common/ReaderList.cpp
	common/RequestHandler.cpp
	common/SCard.cpp
	common/setup.cpp
	common/SignRequestHandler.cpp
	common/test.cpp
	common/UserCertsRequestHandler.cpp
	common/util.cpp
	common/VersionRequestHandler.cpp
	common/VirtualCard.cpp
	common/VirtualReader.cpp
	common/x509Util.c
)

# 4) INCLUDES
#############

target_include_directories(beid
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

# 5) LINK
#########

target_link_libraries(beid
	PRIVATE
		Boost::system
)

if(APPLE)
	target_link_libraries(beid
		PRIVATE
			"-framework PCSC"
			hv_static
		)
endif()

# Installation (optional)
install(TARGETS beid
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
install(FILES beid.h DESTINATION include)
