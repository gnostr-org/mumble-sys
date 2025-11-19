# Mumble macOS Qt6 Build Notes

This document outlines the steps taken to successfully build the Mumble client on macOS with Homebrew-installed Qt6 (version 6.9.3), addressing specific issues related to Qt library linking and header inclusion.

## Problem Overview

The primary challenges encountered during the build process were:
-   `QSqlDatabase.h` not found.
-   `utf8/cpp11.h` not found.
-   `QSvgRenderer` not found.
-   Linker errors for `mumble_drft_init`, `mumble_drft_clear`, and `mumble_drft_forward` (related to `smallft` library).

## Solutions Implemented

The following modifications were made to the CMake configuration files:

### 1. `mumble-sys/CMakeLists.txt`

-   **Smallft Library Definition**:
    To resolve the linker errors for `smallft` functions, the `smallft` library was explicitly defined as a static library.

    ```cmake
    add_library(smallft STATIC
        "${3RDPARTY_DIR}/smallft/smallft.cpp"
        "${3RDPARTY_DIR}/smallft/smallft.h"
    )
    ```

### 2. `mumble-sys/src/mumble/CMakeLists.txt`

-   **Explicit Qt6::Sql Linking**:
    To ensure `QSqlDatabase.h` and other QtSql components were correctly found and linked, `Qt6::Sql` was explicitly added to the target's linked libraries.

    ```cmake
    target_link_libraries(mumble_client_object_lib PUBLIC Qt6::Sql)
    ```

-   **Explicit QtSql, QtConcurrent, and QtSvg Include Paths**:
    Despite `find_package(Qt6 COMPONENTS ...)` being used, the compiler was not correctly locating framework headers for `QtSql`, `QtConcurrent`, and `QtSvg`. Explicit include directories were added for these.

    ```cmake
    target_include_directories(mumble_client_object_lib SYSTEM PRIVATE
        /opt/homebrew/Cellar/qt/6.9.3/lib/QtSql.framework/Headers
        /opt/homebrew/Cellar/qt/6.9.3/lib/QtConcurrent.framework/Headers
        /opt/homebrew/Cellar/qt/6.9.3/lib/QtSvg.framework/Headers
    )
    ```

-   **UTF-8 CPP11 Header Inclusion**:
    The `utf8/cpp11.h` header, part of the `utfcpp` third-party library, was not being found. The source directory for `utfcpp` was added to the include paths.

    ```cmake
    target_include_directories(mumble_client_object_lib PUBLIC
        "${3RDPARTY_DIR}/utfcpp/source"
    )
    ```

-   **Linking Smallft**:
    The newly defined `smallft` static library was linked to `mumble_client_object_lib`.

    ```cmake
    target_link_libraries(mumble_client_object_lib PUBLIC mumble smallft)
    ```

These changes collectively resolved the header not found and linker errors, allowing the Mumble client to build successfully on macOS with Qt6.
