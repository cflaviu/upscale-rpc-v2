import qbs

CppApplication {
    Depends { name: "upscale-rpc" }
    name: "upscale-rpc-unit-tests"
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++20"
    cpp.enableRtti: false
    cpp.includePaths: ["inc", "inc_dep"]
    cpp.libraryPaths: []
    cpp.staticLibraries: [
        "/usr/lib/libCatch2WithMain.a",
    ]
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    files: [
        "inc/types.hpp",
        "src/request/aggregate_test.cpp",
        "src/request/single_test.cpp",
        "src/response/aggregate_test.cpp",
        "src/response/single_test.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
