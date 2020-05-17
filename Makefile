C_SRCS = $(wildcard *.c)
ASM_SRCS = $(wildcard *.S)
OBJS = $(C_SRCS:%.c=%_c.o)
OBJS += $(ASM_SRCS:%.S=%_s.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: kernel8.img clean

%_s.o: %.S
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

%_c.o: %.c
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

kernel8.img: $(OBJS)
	aarch64-linux-gnu-ld -nostdlib -nostartfiles $(OBJS) -T link.ld -o kernel8.elf
	aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img
clean:
	rm kernel8.elf *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio 
runasm:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio -d in_asm
