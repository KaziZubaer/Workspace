hotels = []

n = int(input())
for i in range(0,n) :
	line = input()
	line = line.split(" ")
	cnt = len(line)
	
	hotel = []
	hotel.append(int(line[0]))
	hotel.append(int(line[1]))
	hotel.append(cnt-2)
	hotel.append(line[2:cnt])
	
	hotels.append(hotel)

m = int(input())
for i in range(0,m) :
	line = input()
	line = line.split()
	cnt = len(line)

	money = int(line[0])
	facility = line[1:cnt]
	availableHotel = []
	for j in range(0,n) :
		if (hotels[j][1] > money) :
			continue
		ok = True
		for k in facility :
			if (k not in hotels[j][3]) :
				ok = False
				break
		if (not ok) :
			continue
		availableHotel.append(hotels[j])
	
	availableHotel = sorted(availableHotel, key = lambda h: (-h[2], h[1], h[0]))
	cnt = len(availableHotel)
	for j in range(0,cnt-1) :
		print(str(availableHotel[j][0]), end = " ")
	if (cnt>0) :
		print(str(availableHotel[cnt-1][0]))
	else :
		print("")
