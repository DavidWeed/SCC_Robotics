import fileinput

#origVal = "$GPGLL,3346.83023,N,11748.56933,W,180252.00,A,A*7E" #random example

for line in fileinput.input():
  
  print(line)

  array = line.split(',')

  fullLat = array[1]
  fullLon = array[3]

  latDeg = fullLat[:2]
  lonDeg = fullLon[:3]

  latMin = fullLat[2:4]
  lonMin = fullLon[3:5]

  latSec = str(float(fullLat[4:]) * 60)
  lonSec = str(float(fullLon[5:]) * 60)

  latDir = array[2]
  lonDir = array[4]

  latTu = (latDeg, latMin, latSec, latDir)
  lonTu = (lonDeg, lonMin, lonSec, lonDir)

  print("Latitude = {} degrees {} minutes {} seconds {}".format(*latTu))

  print("Longitude = {} degrees {} minutes {} seconds {}".format(*lonTu))
