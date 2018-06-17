import argparse
import shutil
import os
import sys
import subprocess

printStackTraceOnError = False

def scriptDir():
	return os.path.dirname(os.path.realpath(__file__))

def runScriptCommand(*args):
	print("Running command:", *args)

	result = subprocess.run([*args])

	if not result.returncode == 0:
		print("Command", *args, "returned error code", result.returncode, file=sys.stderr)

def getImmediateSubFolderNames(path):
	if not os.path.exists(path):
		return []

	return [item for item in os.listdir(path) if os.path.isdir(os.path.join(path, item))]

def cleanBuildDirectory(buildDirectory):
	print("Cleaning", buildDirectory)

	if os.path.isdir:
		shutil.rmtree(buildDirectory, ignore_errors=True)
	elif os.path.isfile:
		os.remove(buildDirectory)

	os.makedirs(buildDirectory)

def runCMakeForDependency(name, config, generator):
	print("Running CMake for", name)

	depLibDirectory = os.path.join(scriptDir(), "submodule-libs", name)
	depOrigDirectory = os.path.join(scriptDir(), "submodules", name)

	cleanBuildDirectory(depLibDirectory)

	oldDirectory = os.getcwd()
	os.chdir(depLibDirectory)
	
	if generator != "":
		runScriptCommand("cmake", "-G", generator, "-DCMAKE_BUILD_TYPE=" + config, depOrigDirectory)
	else:
		runScriptCommand("cmake", "-DCMAKE_BUILD_TYPE=" + config, depOrigDirectory)
	
	os.chdir(oldDirectory)

	print("")

def runCMakeForAllDependencies(config, generator):
	depsList = getImmediateSubFolderNames(os.path.join(scriptDir(), "submodules"))

	for item in depsList:
		runCMakeForDependency(item, config, generator)

	print("CMake configuration run for all dependencies - see the submodule-libs directory for output.")
	print("Run your platform-specific build method (eg. make, Visual Studio, msbuild, ...) to build the dependencies.")

def main():
	print("Script directory:", scriptDir())
	print("")

	parser = argparse.ArgumentParser(description="Build helper script.")
	parser.add_argument("--deps-config", help="CMake build configuration for dependencies.", default="Release", choices=["Debug", "Release"])
	parser.add_argument("-G", "--generator", help="Override CMake generator type (see cmake --help).", default="")

	args = parser.parse_args()

	runCMakeForAllDependencies(args.deps_config, args.generator)


if __name__ == "__main__":
	main()
