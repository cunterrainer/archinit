ARCHINIT=src/main.c -o $(ARCHINITBIN)
CHROOT=src/ChrootScript.c -o $(CHROOTBIN)
GCF=src/GatherConfigFiles.c -o $(GCFBIN)
SETUP=src/Setup.c -o $(SETUPTBIN)

ARCHINITBIN=archinit
CHROOTBIN=chroot_script
GCFBIN=gcf
SETUPTBIN=setup

all:
	gcc -static-libgcc $(ARCHINIT)
	gcc -static-libgcc $(CHROOT)
	gcc -static-libgcc $(GCF)
	gcc -static-libgcc $(SETUP)

opt:
	gcc -Os -static-libgcc $(ARCHINIT)
	gcc -Os -static-libgcc $(CHROOT)
	gcc -Os -static-libgcc $(GCF)
	gcc -Os -static-libgcc $(SETUP)

cln:
	rm $(ARCHINITBIN) $(CHROOTBIN) $(GCFBIN) $(SETUPTBIN)
