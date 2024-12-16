import os

directory = r'assets'

out = open('assets.dat', 'wb')
files_list = []
pos = 0

for root, dirs, files in os.walk(directory):
        for f in files:
            name = os.path.join(root, f).replace('\\', '/')
            files_list.append((name, pos))
            size = os.stat(name).st_size
            if '.vert' in name or '.frag' in name or '.comp' in name:
                size += 1
            out.write((name + '$' + str(pos) + '$' + str(pos + size) + '&').encode('utf-8'))
            pos += size

out.write('@META_END&'.encode('utf-8'))
end_of_meta = out.tell()

print(files_list, sep='\n')

for filename, position in files_list:
    f = open(filename, 'rb')
    print(end_of_meta + position)
    out.seek(end_of_meta + position)
    out.write(f.read())
    if '.vert' in filename or '.frag' in filename or '.comp' in filename:
        out.write(b'\0')
    f.close()

out.close()
