all: run

run: faceDetect
	./faceDetect --cascade= --nested-cascade= lena.jpg

faceDetect: faceDetect.cpp
	g++ faceDetect.cpp -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -o faceDetect

clean:
	rm faceDetect
