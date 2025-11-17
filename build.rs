use std::env;
use std::path::PathBuf;

fn main() {
    // build the mumble library
    let mut config = cmake::Config::new(".");

    let dst = config.build();

    println!("cargo:rustc-link-search=native={}/lib/mumble", dst.display());
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
        .blocklist_type("std::.*")
        .blocklist_type("std___1_.*")
        .blocklist_type("__darwin_.*")
        .blocklist_type("__sFILE")
        .blocklist_type("__siginfo")
        .blocklist_type("__sigaction_u")
        .blocklist_type("__float2")
        .blocklist_type("__double2")
        .blocklist_type("__cxxabiv1_.*")
        .blocklist_type("__builtin_va_list")
        .blocklist_type("API::APIPromise")
        .blocklist_type("API::MumbleAPICurator")
        .blocklist_type("API::MumbleAPI")
        .blocklist_type("API::PluginData")
        .allowlist_type("MumbleAPI_v_1_2_0")
        .allowlist_type("Mumble_PluginFeature")
        .allowlist_type("Mumble_TalkingState")
        .allowlist_type("Mumble_TransmissionMode")
        .allowlist_type("Mumble_ErrorCode")
        .allowlist_type("Mumble_PositionalDataErrorCode")
        .allowlist_type("Mumble_SettingsKey")
        .allowlist_type("Mumble_KeyCode")
        .allowlist_type("MumbleVersion")
        .allowlist_type("MumbleStringWrapper")
        .default_non_copy_union_style(bindgen::NonCopyUnionStyle::ManuallyDrop)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
