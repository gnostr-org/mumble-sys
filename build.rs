use std::env;
use std::path::PathBuf;

fn main() {
    // build the mumble library
    let mut config = cmake::Config::new(".");

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=mumble");
    println!("cargo:rustc-link-lib=dylib=c++");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .clang_arg("-std=c++20")
        .clang_arg("--target=aarch64-apple-darwin")
        .clang_arg("-x")
        .clang_arg("c++")
        .clang_arg("-v")
        // Add project-specific include paths
        .clang_arg("-I.") // Add mumble-sys root
        .clang_arg("-I./src") // Add mumble-sys/src
        .clang_arg("-I./src/mumble") // Add mumble-sys/src/mumble
        .clang_arg("-I./plugins") // Add mumble-sys/plugins
        // Add Qt6 include paths based on qmake generated Makefile
        .clang_arg("-I/opt/homebrew/Cellar/qt/6.9.3/lib/QtCore.framework/Headers")
        .clang_arg("-F/opt/homebrew/Cellar/qt/6.9.3/lib")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include")
        .clang_arg("-I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1")
        .blocklist_type("std::memory_order")
        .blocklist_type("std::__1::memory_order")
        .blocklist_type("char_type")
        .blocklist_type("rep")
        .blocklist_type("type_")
        .blocklist_type("std___1___forward_list_node_value_type")
        .blocklist_type("std::.*")
        .blocklist_type("__gnu_cxx::.*")
        .blocklist_type("__.*")
        .blocklist_type("API::APIPromise")
        .blocklist_type("API::MumbleAPICurator")
        .blocklist_type("API::MumbleAPI")
        .blocklist_type("API::PluginData")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
