extern crate bindgen;

use std::env;
use std::path::PathBuf;
use std::process::Command;

fn main() {
    // // Get Homebrew installation prefix for 'ice' package
    // let ice_prefix_output = Command::new("brew")
    //     .arg("--prefix")
    //     .arg("ice")
    //     .output()
    //     .expect("Failed to execute brew command for ice_prefix");
    // let ice_prefix = String::from_utf8(ice_prefix_output.stdout)
    //     .expect("Failed to read stdout for ice_prefix")
    //     .trim()
    //     .to_string();

    // if ice_prefix.is_empty() {
    //     panic!("ERROR: Could not determine Homebrew prefix for 'ice'. Aborting.");
    // }

    // let ice_cmake_dir = format!("{}/share/Ice/cmake", ice_prefix);

    // // Get installation prefix for MySQL client
    // let mysql_prefix_output = Command::new("brew")
    //     .arg("--prefix")
    //     .arg("mysql-client")
    //     .output()
    //     .expect("Failed to execute brew command for mysql_prefix");
    // let mysql_prefix = String::from_utf8(mysql_prefix_output.stdout)
    //     .expect("Failed to read stdout for mysql_prefix")
    //     .trim()
    //     .to_string();

    // if mysql_prefix.is_empty() {
    //     panic!("ERROR: Could not determine Homebrew prefix for 'mysql-client'. Aborting.");
    // }

    // build the mumble library
    let mut config = cmake::Config::new(".");

    // config.define("CMAKE_BUILD_TYPE", "Release");
    // config.define("Ice_DIR", ice_cmake_dir);

    // let cmake_prefix_path = format!("{}/opt/homebrew", mysql_prefix);
    // config.define("CMAKE_PREFIX_PATH", cmake_prefix_path);
    // config.define("MySQL_DIR", mysql_prefix.clone());
    // config.cxxflag(format!("-I{}/include", mysql_prefix));
    // config.cxxflag(format!("-L{}/lib", mysql_prefix));

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=mumble");
    println!("cargo:rustc-link-lib=dylib=c++");

    let mut builder = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-std=c++20")
        .clang_arg("--target=aarch64-apple-darwin")
        .clang_arg("-x")
        .clang_arg("c++")
        .clang_arg("-v");

    // Add project-specific include paths
    builder = builder.clang_arg("-I."); // Add mumble-sys root
    builder = builder.clang_arg("-I./src"); // Add mumble-sys/src
    builder = builder.clang_arg("-I./src/mumble"); // Add mumble-sys/src/mumble
    builder = builder.clang_arg("-I./plugins"); // Add mumble-sys/plugins

    // Add Homebrew and dependency include paths
    // builder = builder.clang_arg(format!("-I{}/include", ice_prefix));
    // builder = builder.clang_arg(format!("-I{}/include", mysql_prefix));

    // Add Qt6 include paths based on qmake generated Makefile
    builder = builder.clang_arg("-I/opt/homebrew/Cellar/qt/6.9.3/lib/QtCore.framework/Headers");
    builder = builder.clang_arg("-F/opt/homebrew/Cellar/qt/6.9.3/lib");

    let bindings = builder
        .blocklist_type("std::memory_order")
        .blocklist_type("std::__1::memory_order")
        .blocklist_type("char_type")
        .blocklist_type("rep")
        .blocklist_type("type_")
        .blocklist_type("std___1___forward_list_node_value_type")
        .blocklist_type("std::.*")
        .blocklist_type("__gnu_cxx::.*")
        .blocklist_type("__.*")
        .default_non_copy_union_style(bindgen::NonCopyUnionStyle::ManuallyDrop);
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
