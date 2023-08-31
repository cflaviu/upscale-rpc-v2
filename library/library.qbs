import qbs

StaticLibrary {
    name: "upscale-rpc"
    Depends { name: 'cpp' }
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++20"
    cpp.enableRtti: false
    cpp.includePaths: ["inc", "inc_dep"]
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    files: [
        "inc/upscale_rpc/basic_types.hpp",
        "inc/upscale_rpc/request/aggregate.hpp",
        "inc/upscale_rpc/request/single.hpp",
        "inc/upscale_rpc/response/aggregate.hpp",
        "inc/upscale_rpc/response/single.hpp",
        "inc/upscale_rpc/utils.hpp",
        "src/upscale_rpc/request/aggregate.cpp",
        "src/upscale_rpc/request/single.cpp",
        "src/upscale_rpc/response/aggregate.cpp",
        "src/upscale_rpc/response/single.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
