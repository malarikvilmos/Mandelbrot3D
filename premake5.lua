workspace "Mandelbrot"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Mandelbrot"
    location "Mandelbrot"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/include/imgui/**.cpp",
        "%{prj.name}/vendor/include/imgui/**.h",
        "%{prj.name}/vendor/include/glad4_3/glad4_3.c",
    }

    includedirs
    {
        "%{prj.name}/vendor/include"
    }

    links
    {
        "SDL2main", "SDL2"
    }

    libdirs
    {
        "%{prj.name}/vendor/lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        kind "ConsoleApp"
        links
        {
            "winmm",
            "version",
            "Imm32",
            "Setupapi"
        }

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines {"_DEBUG", "_CONSOLE", "_CRT_SECURE_NO_WARNINGS"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        kind "WindowedApp"
        defines {"_NDEBUG", "_CONSOLE", "_CRT_SECURE_NO_WARNINGS"}
        runtime "Release"
        optimize "Full"

    filter {}

