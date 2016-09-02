// pseudo iter

list_proto = [].proto

list_proto.iter = fn: {
	list: base,
	cur: 0,
	next: fn: {
		if base.cur < base.list.size():
			loc val = base.list[base.cur]
		else:
			raise "done"

		base.cur = base.cur + 1

		val
	}
}

list = []
i = 0

while i < 100000: {
	list.push(i)
	i = i + 1
}

a = list.iter()

while 1: {
	try: i = a.next()
	catch err:
		break
	if !(i % 1000):
		print(i)
}

// -> "num: 0.000"
// -> "num: 1000.000"
// -> "num: 2000.000"
// -> "num: 3000.000"
// -> "num: 4000.000"
// -> "num: 5000.000"
// -> "num: 6000.000"
// -> "num: 7000.000"
// -> "num: 8000.000"
// -> "num: 9000.000"
// -> "num: 10000.000"
// -> "num: 11000.000"
// -> "num: 12000.000"
// -> "num: 13000.000"
// -> "num: 14000.000"
// -> "num: 15000.000"
// -> "num: 16000.000"
// -> "num: 17000.000"
// -> "num: 18000.000"
// -> "num: 19000.000"
// -> "num: 20000.000"
// -> "num: 21000.000"
// -> "num: 22000.000"
// -> "num: 23000.000"
// -> "num: 24000.000"
// -> "num: 25000.000"
// -> "num: 26000.000"
// -> "num: 27000.000"
// -> "num: 28000.000"
// -> "num: 29000.000"
// -> "num: 30000.000"
// -> "num: 31000.000"
// -> "num: 32000.000"
// -> "num: 33000.000"
// -> "num: 34000.000"
// -> "num: 35000.000"
// -> "num: 36000.000"
// -> "num: 37000.000"
// -> "num: 38000.000"
// -> "num: 39000.000"
// -> "num: 40000.000"
// -> "num: 41000.000"
// -> "num: 42000.000"
// -> "num: 43000.000"
// -> "num: 44000.000"
// -> "num: 45000.000"
// -> "num: 46000.000"
// -> "num: 47000.000"
// -> "num: 48000.000"
// -> "num: 49000.000"
// -> "num: 50000.000"
// -> "num: 51000.000"
// -> "num: 52000.000"
// -> "num: 53000.000"
// -> "num: 54000.000"
// -> "num: 55000.000"
// -> "num: 56000.000"
// -> "num: 57000.000"
// -> "num: 58000.000"
// -> "num: 59000.000"
// -> "num: 60000.000"
// -> "num: 61000.000"
// -> "num: 62000.000"
// -> "num: 63000.000"
// -> "num: 64000.000"
// -> "num: 65000.000"
// -> "num: 66000.000"
// -> "num: 67000.000"
// -> "num: 68000.000"
// -> "num: 69000.000"
// -> "num: 70000.000"
// -> "num: 71000.000"
// -> "num: 72000.000"
// -> "num: 73000.000"
// -> "num: 74000.000"
// -> "num: 75000.000"
// -> "num: 76000.000"
// -> "num: 77000.000"
// -> "num: 78000.000"
// -> "num: 79000.000"
// -> "num: 80000.000"
// -> "num: 81000.000"
// -> "num: 82000.000"
// -> "num: 83000.000"
// -> "num: 84000.000"
// -> "num: 85000.000"
// -> "num: 86000.000"
// -> "num: 87000.000"
// -> "num: 88000.000"
// -> "num: 89000.000"
// -> "num: 90000.000"
// -> "num: 91000.000"
// -> "num: 92000.000"
// -> "num: 93000.000"
// -> "num: 94000.000"
// -> "num: 95000.000"
// -> "num: 96000.000"
// -> "num: 97000.000"
// -> "num: 98000.000"
// -> "num: 99000.000"