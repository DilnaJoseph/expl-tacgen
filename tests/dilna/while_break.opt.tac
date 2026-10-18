i = 0
L1:
if i >= 10 goto L4
t1 = i % 2
if t1 == 0 goto L3
goto L2
L2:
i = i + 1
goto L1
L3:
goto L4
goto L1
L4:
write i
