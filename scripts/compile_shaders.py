# Note: compile_shaders.py is a temporary solution that will be replaced by a more sophisticated solution once needed.

import sys
import os
from os import listdir
from os import path

import shutil


metal_extensions = [".metal"]
glsl_extensions = [".frag", ".vert"]


def isFileType(filepath, extensions):
    for extension in extensions:
        if filepath.endswith(extension):
            return True
    return False


def isMetal(filepath):
    return isFileType(filepath, metal_extensions)


def isGlsl(filepath):
    return isFileType(filepath, glsl_extensions)


def isShader(filepath):
    return isMetal(filepath) or isGlsl(filepath)


def error(message):
    print("[ERROR] " + message)
    exit(1)


def prepareTargetDirectory(directory_path):

    # if the target directory doesn't exist, create it
    if not path.exists(directory_path):
        os.makedirs(directory_path)
        return  # success

    if not path.isdir(directory_path):
        error(directory_path + " is an existing file, not a directory")

    # directory exists already, see if it is empty
    items = listdir(directory_path)

    if len(items) > 0:
        print("Directory \"" + directory_path + "\" is not empty, containing the following files:")
        print(items)
        answer = input("Do you want to empty the folder? [Y/N]")
        answer = answer.lower()
        if answer.startswith('y') or answer == "":
            shutil.rmtree(directory_path)
            os.makedirs(directory_path)
        else:
            error("did not empty folder")


def main():
    if (len(sys.argv)) != 4:
        error("invalid amount of arguments, usage: " + path.basename(__file__) +
              " <source directory> <target directory>")

    shaders_source_directory = os.path.abspath(sys.argv[1])
    print("source: " + shaders_source_directory)

    shaders_target_directory = os.path.abspath(sys.argv[2])
    print("target: " + shaders_target_directory)

    metal_library_target = os.path.abspath(sys.argv[3])
    print("library target: " + metal_library_target)

    if not path.isdir(shaders_source_directory):
        error("source directory is not a directory")

    prepareTargetDirectory(shaders_target_directory)
    compileShaders(shaders_source_directory, shaders_target_directory)
    createMetalLibrary(shaders_target_directory, metal_library_target)


# converts shaders to intermediate representation (for metal we also need to call createMetalLibrary
def compileShaders(source_directory, target_directory):
    # compile all shaders
    for root, subdirs, files in os.walk(source_directory):

        shader_files = [f for f in files if isShader(f)]

        if len(shader_files) == 0:
            continue

        for shader_file in shader_files:

            source_file = path.join(root, shader_file)

            relative_dir = root[(len(source_directory)+1):]
            if len(relative_dir) > 0:
                relative_dir = relative_dir.replace(os.path.sep, '_')
                relative_dir += "_"

            shader_file_name = shader_file.split('.')[0]
            target_file = path.join(target_directory, relative_dir + shader_file_name)

            if isMetal(shader_file):
                compileMetal(source_file, target_file)
            elif isGlsl(shader_file):
                compileGlslToSpirV(source_file, target_file)


# target is without extension
def compileMetal(source, target):
    target += ".ir"
    print("compiling metal shader from \"" + source + "\" to \"" + target + "\"")
    os.system("xcrun -sdk macosx metal -o " + target + " -c " + source)


# takes all .ir files in the source_directory and converts into one library with a given path
# this path does not contain the file extension
def createMetalLibrary(source_directory, target):
    target += ".metallib"

    files = listdir(source_directory)
    ir_files = [f for f in files if f.endswith(".ir")]

    if len(ir_files) == 0:
        print("could not create metal library, as the provided directory \"" + source_directory
              + "\" does not contain any .ir files")
        return

    print("creating metal library at \"" + target + "\" from:")
    print(ir_files)

    command = "xcrun -sdk macosx metallib -o \"" + target + "\" "
    for ir_file in ir_files:
        command += "\"" + path.join(source_directory, ir_file) + "\" "
    os.system(command)

# target is without extension
def compileGlslToSpirV(source, target):
    print("todo")
    pass


if __name__ == "__main__":
    main()