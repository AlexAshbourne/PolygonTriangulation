import matplotlib.pyplot as plt
from sys import argv


inFilename  = argv[1]
inFile  = open(inFilename, "rb")

line = inFile.readline().split()
while not line[0] == "LINE":
	line = inFile.readline().split()
# read in until you get to vertices or edges
print "read all lines"
line = inFile.readline().split()

while not line == []:
	x_coords=[]
	y_coords=[]
	while not line[0] == "LINE":
		# print line
		x_coords.append(line[0])
		y_coords.append(line[1])
		line = inFile.readline().split()
		if line == []:
			break

	if len(x_coords) == 1: #this is vertex
		plt.scatter(x_coords, y_coords,  c='r', s = 30)
	else: #this is an edge
		plt.plot(x_coords, y_coords, 'k-', linewidth= 1)


	line = inFile.readline().split()



plt.axes().set_aspect('equal', 'datalim')
plt.axis('off')
plt.show()