extern crate bindgen;

use std::env;
use std::path::PathBuf;
use std::process::Command;

fn find_xcode_sdk_path() -> Option<String> {
    let output = Command::new("xcrun")
        .arg("--show-sdk-path")
        .output()
        .ok()?;

    if output.status.success() {
        String::from_utf8(output.stdout)
            .ok()
            .map(|s| s.trim().to_string())
    } else {
        None
    }
}

fn main() {
    // Get Homebrew installation prefix for 'ice' package
    let ice_prefix_output = Command::new("brew")
        .arg("--prefix")
        .arg("ice")
        .output()
        .expect("Failed to execute brew command for ice_prefix");
    let ice_prefix = String::from_utf8(ice_prefix_output.stdout)
        .expect("Failed to read stdout for ice_prefix")
        .trim()
        .to_string();

    if ice_prefix.is_empty() {
        panic!("ERROR: Could not determine Homebrew prefix for 'ice'. Aborting.");
    }

    let ice_cmake_dir = format!("{}/share/Ice/cmake", ice_prefix);

    // Get installation prefix for MySQL client
    let mysql_prefix_output = Command::new("brew")
        .arg("--prefix")
        .arg("mysql-client")
        .output()
        .expect("Failed to execute brew command for mysql_prefix");
    let mysql_prefix = String::from_utf8(mysql_prefix_output.stdout)
        .expect("Failed to read stdout for mysql_prefix")
        .trim()
        .to_string();

    if mysql_prefix.is_empty() {
        panic!("ERROR: Could not determine Homebrew prefix for 'mysql-client'. Aborting.");
    }

    // build the mumble library
    let mut config = cmake::Config::new(".");

    config.define("CMAKE_BUILD_TYPE", "Release");
    config.define("Ice_DIR", ice_cmake_dir);

    let cmake_prefix_path = format!("{};/opt/homebrew", mysql_prefix);
    config.define("CMAKE_PREFIX_PATH", cmake_prefix_path);
    config.define("MySQL_DIR", mysql_prefix.clone());
    config.cxxflag(format!("-I{}/include", mysql_prefix));
    config.cxxflag(format!("-L{}/lib", mysql_prefix));

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=mumble");
    println!("cargo:rustc-link-lib=dylib=c++");

    let mut builder = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-std=c++20")
        .clang_arg("--target=aarch64-apple-darwin");

    // Add project-specific include paths
    builder = builder.clang_arg("-I."); // Add mumble-sys root
    builder = builder.clang_arg("-I./src"); // Add mumble-sys/src
    builder = builder.clang_arg("-I./src/mumble"); // Add mumble-sys/src/mumble
    builder = builder.clang_arg("-I./plugins"); // Add mumble-sys/plugins

    // Add Homebrew and dependency include paths
    builder = builder.clang_arg("-I/opt/homebrew/include");
    builder = builder.clang_arg(format!("-I{}/include", ice_prefix));
    builder = builder.clang_arg(format!("-I{}/include", mysql_prefix));

    // Add Xcode SDK include paths
    if let Some(sdk_path) = find_xcode_sdk_path() {
        builder = builder.clang_arg(format!("-I{}/usr/include", sdk_path));
        builder = builder.clang_arg(format!("-I{}/System/Library/Frameworks/Kernel.framework/Versions/A/Headers", sdk_path));
        builder = builder.clang_arg(format!("-I{}/usr/include/c++/v1", sdk_path));
    }

    let bindings = builder
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
