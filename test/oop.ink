a = 10
a.+ = fn b: base - b
a.* = "hi"

print(a + 5)
try: a * 5
catch err: print(err.msg)

res = 1

a.<= = fn b: res
a.>= = fn b: a <= b
a.< = fn b: a >= b
a.> = fn b: a < b

try: 10 + ""

if a > 11: print("yes!")
else: print("no!")

res = 0

if a > 11: print("no!")
else: print("yes!")

a = [1, 2, 3]

a.[] = fn idx: {
	if typeof(idx) == "list":
		for i in idx:
			print(i)
	else:
		print(typeof(idx))
}

a[1, 2, 3]
a[]

// -> "num: 5"
// -> "str: unable to invoke object of type <string>"
// -> "str: yes!"
// -> "str: yes!"
// 
// -> "num: 1"
// -> "num: 2"
// -> "num: 3"
//
// -> "str: none"
