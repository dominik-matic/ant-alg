CC=g++
CLAGS=-Ofast
FILES=main.cpp AntAlg.cpp

main: clean
	$(CC) $(CFLAGS) $(FILES) -o $@

clean:
ifneq (,$(wilcard ./main))
	rm main
endif