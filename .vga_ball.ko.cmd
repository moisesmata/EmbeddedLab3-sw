cmd_/root/Lab3Test/EmbeddedLab3-sw/vga_ball.ko := ld -r  -EL -T ./scripts/module-common.lds -T ./arch/arm/kernel/module.lds  --build-id  -o /root/Lab3Test/EmbeddedLab3-sw/vga_ball.ko /root/Lab3Test/EmbeddedLab3-sw/vga_ball.o /root/Lab3Test/EmbeddedLab3-sw/vga_ball.mod.o ;  true
