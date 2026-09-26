savedcmd_ldd_20261001_kernelBuild.mod := printf '%s\n'   ldd_20261001_kernelBuild.o | awk '!x[$$0]++ { print("./"$$0) }' > ldd_20261001_kernelBuild.mod
