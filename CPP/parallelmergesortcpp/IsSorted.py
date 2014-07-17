infile = open('output.out')
content = infile.read()
content = content.split()
i = 1
while True:
    if int(content[i])>=int(content[i-1]):
        if i == len(content) - 1:
                print ('Sorted')
                break
    else:
        print('Not Sorted')
        break
    i+=1
