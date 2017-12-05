def myMap (x, inMin, inMax, outMin, outMax):
        return (x -inMin) * (outMax - outMin) / (inMax - inMin) + outMin


orig_h = 355
orig_s = 78
orig_v = 74


new_h = myMap(orig_h, 0, 360, 0, 179)
new_s = myMap(orig_s, 0, 100, 0, 255)
new_v = myMap(orig_v, 0, 100, 0, 255)

print "h = "
print new_h
print "s = "
print new_s
print "v = "
print new_v
