infile = open('output.out')
content = infile.read()
content = content.split()
infile = open('testinput.in')
content2 = infile.read()
content2 = content2.split()

print len(content)
print len(content2)
for x in range(1000):
	content2.remove(content[x])
print content2
