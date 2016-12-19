import math
import random
from PIL import Image

f = open("bisecting.txt","r")
n = 0
points = []
clusters = []
K_VAL = 15
ITERATION = 60

class Point :

	def __init__ (self, _x=0, _y=0) :
		self.x = _x
		self.y = _y
		self.cluster = []
		
	def clear (self) :
		self.cluster = []
	
	def addPoint (self, p) :
		self.cluster.append(p)
	
	def dist2 (self, p) :
		return (self.x-p.x)*(self.x-p.x)+(self.y-p.y)*(self.y-p.y)
	
	def add (self, p) :
		self.x += p.x
		self.y += p.y
		
	def calculateSelf (self) :
		oldX = self.x
		oldY = self.y
		self.x = self.y = 0
		for p in self.cluster :
			self.add(p)
		if len(self.cluster) > 0 :
			self.x /= len(self.cluster)
			self.y /= len(self.cluster)
		return abs(oldX-self.x)+abs(oldY-self.y)
	
	def SSE (self) :
		result = 0
		for p in self.cluster :
			result += self.dist2(p)
		return result
	
#basic k means
def basicKMeans (pointsToCluster, k, clusterCenters) :
	if len(clusterCenters) == 0 :
		miniX = pointsToCluster[0].x
		miniY = pointsToCluster[0].y
		maxiX = pointsToCluster[0].x
		maxiY = pointsToCluster[0].y
		for i in range (1, len(pointsToCluster)) :
			miniX = min(miniX, pointsToCluster[i].x)
			miniY = min(miniY, pointsToCluster[i].y)
			maxiX = max(maxiX, pointsToCluster[i].x)
			maxiY = max(maxiY, pointsToCluster[i].y)
		for i in range (0, k) :
			clusterCenters.append(Point(random.uniform(miniX, maxiX), random.uniform(miniY, maxiY)))
	maxDiff = 1e8
	while maxDiff > 1e-8 :
		for c in clusterCenters :
			c.clear()
		for p in pointsToCluster :
			closest = 0
			closeDist = p.dist2(clusterCenters[0])
			for i in range (1, k) :
				temp = p.dist2(clusterCenters[i])
				if temp < closeDist :
					closeDist = temp
					closest = i
			clusterCenters[closest].addPoint(p)
		maxDiff = 1e-15
		for c in clusterCenters :
			maxDiff = max(maxDiff, c.calculateSelf())
	return clusterCenters
		
#read points
while True :
	line = f.readline()
	if not line :
		break
	line = line.split("\t")
	n += 1
	points.append(Point(float(line[0]), float(line[1])))
	
#make the first cluster with all points
clusters.append(Point())
for p in points :
	clusters[0].addPoint(p)
clusters[0].calculateSelf()

#run bisecting k means
while len(clusters) < K_VAL :
	selected = 0
	maxSSE = clusters[0].SSE()
	for i in range (1, len(clusters)) :
		tempSSE = clusters[i].SSE()
		if tempSSE > maxSSE :
			maxSSE = tempSSE
			selected = i
	clusterToBisect = clusters[selected]
	del clusters[selected]
	clusterCenters = basicKMeans(clusterToBisect.cluster, 2, [])
	lowestSSE = clusterCenters[0].SSE() + clusterCenters[1].SSE()
	for i in range (1, ITERATION) :
		tempClusterCenters = basicKMeans(clusterToBisect.cluster, 2, [])
		tempSSE = tempClusterCenters[0].SSE() + tempClusterCenters[1].SSE()
		if tempSSE < lowestSSE :
			clusterCenters = tempClusterCenters
			lowestSSE = tempSSE	
	clusters.append(clusterCenters[0])
	clusters.append(clusterCenters[1])

#run basic k means
clusters = basicKMeans(points, K_VAL, clusters)

#show in image
miniX = points[0].x
miniY = points[0].y
maxiX = points[0].x
maxiY = points[0].y
for i in range (1, len(points)) :
	miniX = min(miniX, points[i].x)
	miniY = min(miniY, points[i].y)
	maxiX = max(maxiX, points[i].x)
	maxiY = max(maxiY, points[i].y)
for i in range (1, len(clusters)) :
	miniX = min(miniX, clusters[i].x)
	miniY = min(miniY, clusters[i].y)
	maxiX = max(maxiX, clusters[i].x)
	maxiY = max(maxiY, clusters[i].y)
miniX -= 1
miniY -= 1
img = Image.new( 'RGB', (540,280), "white")
pixels = img.load()
for p in points :
	i = min(511,int(((p.x-miniX)*512)/(maxiX-miniX)))
	j = min(255,int(((p.y-miniY)*256)/(maxiY-miniY)))
	pixels[i, j] = (0, 0, 0)
for p in clusters :
	i = min(511,int(((p.x-miniX)*512)/(maxiX-miniX)))
	j = min(255,int(((p.y-miniY)*256)/(maxiY-miniY)))
	for k in range (-1, 2) :
		for l in range (-1, 2) :
			pixels[i+k, j+l] = (255, 0, 0)
img.show()

