import sys
from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.cmake import CMake


class BreakoutGame(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    options = {"test": [True, False]}

    default_options = {"test": True}

    def requirements(self):
        self.requires("sfml/2.6.1")
        self.test_requires("gtest/1.11.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.options.test:
            cmake.build(target="run_test")
