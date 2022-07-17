mkfifo BtoA -m700
./main_tester.out < BtoA | ./main.out > BtoA
rm BtoA

