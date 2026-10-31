-----------------------------------------------------
Userspace:
Normal C application

gcc hello.c -o hello
./hello


-----------------------------------------------------
Kernel space:
Linux Device Driver

make // run make from where the MakeFile is present 

sudo insmod ldd_20261002_charDevice.ko
// to insert the compiled driver to kernel


-----------------------------------------------------
make
// Build the kernel module.
// (compiles the driver .c file and generates the 
required object files)

sudo insmod ldd_20261001_helloModule.ko
// Load the driver

lsmod | grep ldd_20261001_helloModule
// Check that it loaded

dmesg | tail -20
// Check kernel messages

sudo rmmod ldd_20261001_helloModule
// Remove the driver

dmesg | tail -20
// Check kernel messages

make clean
// Clean the project


-----------------------------------------------------
modinfo your_module.ko
// to view module metadata


-----------------------------------------------------




-----------------------------------------------------




-----------------------------------------------------
