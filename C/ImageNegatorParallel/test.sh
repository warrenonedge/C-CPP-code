#!/bin/sh

lamboot

./snegate image.ppm negative.ppm >> out.txt
mpirun -np 1 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 2 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 3 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 4 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 6 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 8 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 12 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 16 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 24 ./pnegate image.ppm negative.ppm >> out.txt
mpirun -np 32 ./pnegate image.ppm negative.ppm >> out.txt

lamhalt

