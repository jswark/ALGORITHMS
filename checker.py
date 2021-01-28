n = 48 #cities
ans = "20 38 9 47 3 34 25 23 31 44 1 40 28 4 24 12 13 14 29 10 21 39 15 37 8 7 35 27 36 42 16 30 0 6 5 45 22 33 2 18 " \
      "43 11 19 26 17 46 32 41 20 "

#check len
if len(ans.split()) == n + 1:
    print("ok1")
else:
    print("neOk1")

#check cicle
answ = ans.split()
if answ[0] == answ[n]:
    print("ok2")
else:
    print("neOk2")

#check all num in answ
for i in range(n):
    if str(i) in answ:
        continue
    else:
        print("neOk3", i)
print("ok3")
