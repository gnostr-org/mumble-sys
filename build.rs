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

    let mut builder = bindgen::Builder::default()\n        .header(\"wrapper.h\")\n        .clang_arg(\"-std=c++20\")\n        .clang_arg(\"--target=aarch64-apple-darwin\");\n\n    // Add project-specific include paths\n    builder = builder.clang_arg(\"-I.\"); // Add mumble-sys root\n    builder = builder.clang_arg(\"-I./src\"); // Add mumble-sys/src\n    builder = builder.clang_arg(\"-I./src/mumble\"); // Add mumble-sys/src/mumble\n    builder = builder.clang_arg(\"-I./plugins\"); // Add mumble-sys/plugins\n\n    // Get default clang include paths\n    let clang_output = Command::new(\"clang\")\n        .arg(\"-v\")\n        .arg(\"-E\")\n        .arg(\"-x\")\n        .arg(\"c++\")\n        .arg(\"/dev/null\")\n        .output()\n        .expect(\"Failed to execute clang command\");\n\n    let stderr = String::from_utf8_lossy(&clang_output.stderr);\n\n    for line in stderr.lines() {\n        if line.starts_with(\"#include <...\> search starts here:\") {\n            // Start parsing include paths\n            for include_path_line in stderr.lines().skip_while(|&l| l != line).skip(1) {\n                if include_path_line.starts_with(\"End of search list.\") {\n                    break;\n                }\n                let path = include_path_line.trim();\n                if !path.is_empty() {\n                    builder = builder.clang_arg(format!(\"-I{}\", path));\n                }\n            }\n            break;\n        }\n    }\n\n    let bindings = builder\n        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))\n        .generate()\n        .expect(\"Unable to generate bindings\");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
