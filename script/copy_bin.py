import os
import shutil
Import("env","projenv")

path = projenv['PROJECT_BUILD_DIR']+'\\' + projenv['PIOENV']+'\\'+projenv['PROGNAME']


def after_build_bin(source, target, env):
    print("copying firmware.bin")
    shutil.copyfile('.pio/build/esp12e/firmware.bin', 'bin/firmware.bin')

def after_build_elf(source, target, env):
    print("copying firmware.elf")
    shutil.copyfile('.pio/build/esp12e/firmware.elf', 'bin/firmware.elf')

if not os.path.exists('bin'):
    os.makedirs('bin')

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", after_build_bin)
env.AddPostAction("$BUILD_DIR/${PROGNAME}.elf", after_build_elf)


