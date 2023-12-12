# Note: compile_shaders.py is a temporary solution that will be replaced by a more sophisticated solution once needed.

import sys
import os
from os import listdir
from os import path


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
        error("directory is not empty")

def main():
    if (len(sys.argv)) != 3:
        error("invalid amount of arguments, usage: " + path.basename(__file__) +
              " <source directory> <target directory>")

    shaders_source_directory = os.path.abspath(sys.argv[1])
    print("source: " + shaders_source_directory)

    shaders_target_directory = os.path.abspath(sys.argv[2])
    print("target: " + shaders_target_directory)

    if not path.isdir(shaders_source_directory):
        error("source directory is not a directory")

    prepareTargetDirectory(shaders_target_directory)

    for root, subdirs, files in os.walk(shaders_source_directory):

        shader_files = [f for f in files if isShader(f)]

        if len(shader_files) == 0:
            continue

        for shader_file in shader_files:

            source_file = path.join(root, shader_file)

            relative_dir = root[(len(shaders_source_directory)+1):]
            if len(relative_dir) > 0:
                relative_dir = relative_dir.replace(os.path.sep, '_')
                relative_dir += "_"

            shader_file_name = shader_file.split('.')[0]
            target_file = path.join(shaders_target_directory, relative_dir + shader_file_name)

            if isMetal(shader_file):
                compileMetal(source_file, target_file)
            elif isGlsl(shader_file):
                compileGlslToSpirV(source_file, target_file)

            #print(relative_dir)

            #print(shader_file)


# target is without extension
def compileMetal(source, target):
    target += ".ir"
    print("compiling metal shader from \"" + source + "\" to \"" + target + "\"")
    os.system("xcrun -sdk macosx metal -o " + target + " -c " + source)

# target is without extension
def compileGlslToSpirV(source, target):
    pass

# files = [f for f in listdir(shaders_source_directory) if path.isfile(path.join(shaders_source_directory, f))] # only files
    # if len(files) == 0:
    #     return error("source directory does not contain any files")
    #
    # glsl_files = [f for f in files if (f.endswith(".frag") or f.endswith(".vert"))]
    #
    # for glsl_file in glsl_files:
    #
    #     print(glsl_file)
    #
    # shaders_target_directory = sys.argv[2]
    # print("target: " + shaders_target_directory)


if __name__ == "__main__":
    main()