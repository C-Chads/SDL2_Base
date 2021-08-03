.ONESHELL:
all: pdtextures
clean:
	rm -f pdtextures/512x512/*
	rm -f pdtextures/256x256/*
	rm -f pdtextures/1024x1024/*
pdtextures:
	@cd pdtextures && ./convert.sh && cd ..