ARCHINIT=src/main.c -o $(ARCHINITBIN)
CHROOT=src/ChrootScript.c -o $(CHROOTBIN)
GCF=src/GatherConfigFiles.c -o $(GCFBIN)
SETUP=src/Setup.cpp -o $(SETUPTBIN)
VMTools=src/VMwareTools.cpp -o $(VMToolsBIN)

ARCHINITBIN=archinit
CHROOTBIN=chroot_script
GCFBIN=gcf
SETUPTBIN=setup
VMToolsBIN=install_vmware_tools

all:
	gcc -static-libgcc $(ARCHINIT)
	gcc -static-libgcc $(CHROOT)
	gcc -static-libgcc $(GCF)
	g++ -static-libgcc $(SETUP)
	g++ -static-libgcc $(VMTools)

opt:
	gcc -Os -static-libgcc $(ARCHINIT)
	gcc -Os -static-libgcc $(CHROOT)
	gcc -Os -static-libgcc $(GCF)
	g++ -Os -static-libgcc $(SETUP)
	g++ -Os -static-libgcc $(VMTools)

cln:
	rm $(ARCHINITBIN) $(CHROOTBIN) $(GCFBIN) $(SETUPTBIN) $(VMToolsBIN)
