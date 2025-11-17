extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    // build the mumble library
    let mut config = cmake::Config::new(".");

    if let Ok(mysql_prefix) = env::var("MYSQL_PREFIX") {
        let cmake_prefix_path = format!("{};/opt/homebrew", mysql_prefix);
        config.define("CMAKE_PREFIX_PATH", cmake_prefix_path);
        config.define("MySQL_DIR", mysql_prefix.clone());
        config.cxxflag(format!("-I{}/include", mysql_prefix));
        config.cxxflag(format!("-L{}/lib", mysql_prefix));
    }

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=mumble");
    println!("cargo:rustc-link-lib=dylib=c++");


    let bindings = bindgen::Builder::default()
        .header("src/mumble/MumbleAPI_structs.h")
        .clang_arg("-I./plugins")
        .clang_arg("-std=c++20")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/include")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include")
        .clang_arg("--target=aarch64-apple-darwin")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
