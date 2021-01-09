ECHFS = ./echfs/echfs-utils

all:
	@echo "install-limine : installs limine"
	@echo "install-echfs : installs echfs"
	@echo "iso : produces an ISO file"
	@echo "boot : boots the os"

iso:
	dd if=/dev/zero bs=1M count=0 seek=64 of=./JOYOS.IMG
	parted -s JOYOS.IMG mklabel msdos
	parted -s JOYOS.IMG mkpart primary 1 100%
	$(ECHFS) -m -p0 JOYOS.IMG quick-format 32768
	$(ECHFS) -m -p0 JOYOS.IMG import JOYOS.BIN boot/JOYOS.BIN
	$(ECHFS) -m -p0 JOYOS.IMG import limine.cfg boot/limine.cfg
	./limine/limine-install ./limine/limine.bin JOYOS.IMG

install-limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v0.7.2
	make limine-install -C ./limine

install-echfs:
	git clone https://github.com/echfs/echfs.git
	make echfs-utils -C ./echfs

boot:
	objdump JOYOS.BIN -dS -Mintel > dump.txt
	qemu-system-x86_64 -enable-kvm -d int -no-reboot -m 2G -hda JOYOS.IMG

clean:
	rm JOYOS.IMG
