savedcmd_ldd_20261001_helloModule.mod := printf '%s\n'   ldd_20261001_helloModule.o | awk '!x[$$0]++ { print("./"$$0) }' > ldd_20261001_helloModule.mod
