cmake_minimum_required(VERSION 3.28)
project(here_is_johnny_v)

set(CMAKE_CXX_STANDARD 26)

add_executable(here_is_johnny_v main.c
        src/upkd/upkd.c
        src/inotify_observer/inotify_observer.c
        src/inotify_observer/inotify_subject.c
        src/utils/file_operations.c
        include/upkd/upkd.h
        src/upkd/ufw_log_entry.c
        include/upkd/ufw_log_entry.h
        src/upkd/db.c
        include/upkd/db.h
        include/upkd/commands/fs_monitor_commands.h
        src/upkd/commands/fs_monitor_commands.c
        src/upkd/commands/file_transfer_commands.c
        include/upkd/commands/file_transfer_commands.h
        include/packet_builder/packet_builder.h
        src/packet_builder/packet_builder.c
        include/data_noise/data_noise.h
        src/data_noise/data_noise.c
        src/upkd/auth.c
        include/upkd/auth.h
        include/crypto/crypto.h
        src/crypto/crypto.c
        src/upkd/data_processor.c
        include/upkd/data_processor.h
        src/upkd/commands/system_commands.c
        include/upkd/commands/system_commands.h
)

# Find the libnet library
find_library(LIBNET_LIBRARY NAMES net)

# If libnet library is found, link it to the executable
if(LIBNET_LIBRARY)
    target_link_libraries(here_is_johnny_v ${LIBNET_LIBRARY})
else()
    # Display an error message and stop if libnet is not found
    message(FATAL_ERROR "libnet not found")
endif()

# Find the OpenSSL library
find_package(OpenSSL REQUIRED)

# If OpenSSL library is found, link it to the executable
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})

    target_link_libraries(here_is_johnny_v ${OPENSSL_LIBRARIES})
else()
    # Display an error message and stop if OpenSSL is not found
    message(FATAL_ERROR "OpenSSL not found")
endif()