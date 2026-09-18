echo "================================="
echo " i.MX6ULL Automatic Boot"
echo "================================="

echo "Loading Linux kernel..."
fatload mmc 0 ${loadaddr} zImage

echo "Loading Device Tree..."
fatload mmc 0 ${fdt_addr} mys-6ull-14x14-gpmi-weim.dtb

echo "Setting boot arguments..."
setenv bootargs "console=ttymxc0,115200 root=/dev/mmcblk0p2 rw rootwait"

echo "Booting Linux..."
bootz ${loadaddr} - ${fdt_addr}
