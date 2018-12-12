mov r0, #20
mov r1, #50
mov r3, #1
mov r2, r1, lsl r3
mov r2, r2, lsl #1
ldr r2, [r3, #256]
add r2, r2, #50
mov r5, #-268435455
mov r5, r5, asr #5