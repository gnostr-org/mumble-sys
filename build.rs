extern crate bindgen;

use std::env;
use std::path::PathBuf;

fn main() {
    // build the mumble library
    let mut config = cmake::Config::new(".");

    if let Ok(mysql_prefix) = env::var("MYSQL_PREFIX") {
        let cmake_prefix_path = format!("{};/opt/homebrew", mysql_prefix);
        config.define("CMAKE_PREFIX_PATH", cmake_prefix_path);
        config.define("MySQL_DIR", mysql_prefix);
        config.cxxflag(format!("-I{}/include", mysql_prefix));
        config.link_flag(format!("-L{}/lib", mysql_prefix));
    }

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=mumble");
    println!("cargo:rustc-link-lib=dylib=c++");


    let bindings = bindgen::Builder::default()
        .header("src/mumble/API.h")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
