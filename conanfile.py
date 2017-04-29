from conans import ConanFile, CMake, tools
import os

class InjeqtConan(ConanFile):
    name = "injeqt"
    version = "0.1"
    description = "Dependency injection framework for Qt."
    license = "GNU Lesser General Public License v2.1 (LGPL-2.1)"
    url = "https://github.com/borco/conan-injeqt"
    settings = "os", "compiler", "build_type", "arch"
    options = {"tests": [True, False], "examples": [True, False]}
    default_options = """
tests=False
examples=False
"""
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/vogel/injeqt")
        self.run("cd injeqt && git checkout 1.2.0")

    def build(self):
        cmake = CMake(self)
        tests = "" if self.options.tests else "-DDISABLE_TESTS=ON"
        examples = "" if self.options.examples else "-DDISABLE_EXAMPLES=ON"
        cmd = 'cmake -DCMAKE_INSTALL_LIBDIR=. -DCMAKE_INSTALL_BINDIR=. injeqt %s %s %s' % (cmake.command_line, tests, examples)
        print(cmd)
        self.run(cmd)
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="injeqt/include")
        self.copy("*injeqt.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["injeqt"]
