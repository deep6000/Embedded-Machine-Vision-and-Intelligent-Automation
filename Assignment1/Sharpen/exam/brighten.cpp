#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>



#define IMG_HEIGHT (320)
#define IMG_WIDTH (240)
#define NUM_ROW_THREADS (4)
#define NUM_COL_THREADS (3)
#define IMG_H_SLICE (IMG_HEIGHT/NUM_ROW_THREADS)
#define IMG_W_SLICE (IMG_WIDTH/NUM_COL_THREADS)


typedef struct _threadArgs
{
    int thread_idx;
    int i;
    int j;
    int h;
    int w;
} threadArgsType;

pthread_t threads[NUM_ROW_THREADS*NUM_COL_THREADS];

threadArgsType threadarg[NUM_ROW_THREADS*NUM_COL_THREADS];
pthread_attr_t fifo_sched_attr;
pthread_attr_t orig_sched_attr;
struct sched_param fifo_param;

using namespace cv; using namespace std;

double alpha=1.0;  int beta=10;  /* contrast and brightness control */


void *brighten_thread(void *threadptr)
{
    
     for( int y = thargs.i; y <(thargs.i+thargs.h); y++ )
    {

        for( int x = thargs.j; x < (thargs.j+thargs.w); x++ )
        { 
            for( int c = 0; c < 3; c++ )
                new_image.at<Vec3b>(y,x)[c] =
                    saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
        }
    }

}

int main( int argc, char** argv )
{
    unsigned int thread_idx;
    Mat image = imread( argv[1] ); // read in image file
    Mat new_image = Mat::zeros( image.size(), image.type() );
    std::cout<<"* Enter alpha brighten factor [1.0-3.0]: ";std::cin>>alpha;
    std::cout<<"* Enter beta contrast increase value [0-100]: "; std::cin>>beta;


     for(thread_idx=0; thread_idx<(NUM_ROW_THREADS*NUM_COL_THREADS); thread_idx++)
    {
	
        if(thread_idx == 0) {idx=1; jdx=1;}
        if(thread_idx > 0 && thread_idx <= 2 ) {idx=1; jdx=(thread_idx*(IMG_W_SLICE-1));}
        if(thread_idx  >= 3 && thread_idx <= 5 ) {idx=IMG_H_SLICE; jdx=(thread_idx*(IMG_W_SLICE-1));}
		if(thread_idx  >= 6 && thread_idx <= 8 ) {idx=(2*(IMG_H_SLICE-1)); jdx=(thread_idx*(IMG_W_SLICE-1));}
		if(thread_idx  >= 9 && thread_idx <= 11 ) {idx=(3*(IMG_H_SLICE-1)); jdx=(thread_idx*(IMG_W_SLICE-1));}	

        threadarg[thread_idx].i=idx;      
        threadarg[thread_idx].h=IMG_H_SLICE-1;        
        threadarg[thread_idx].j=jdx;        
        threadarg[thread_idx].w=IMG_W_SLICE-1;

        //printf("create thread_idx=%d\n", thread_idx);    
        pthread_create(&threads[thread_idx], (void *)0, brighten_thread, (void *)&threadarg[thread_idx]);

    }


    for(thread_idx=0; thread_idx<(NUM_ROW_THREADS*NUM_COL_THREADS); thread_idx++)
    {
            //printf("join thread_idx=%d\n", thread_idx);    
            if((pthread_join(threads[thread_idx], (void **)0)) < 0)
                perror("pthread_join");
    }


    namedWindow("Original Image", 1); namedWindow("New Image", 1);
    imshow("Original Image", image); imshow("New Image", new_image);
    waitKey(); return 0;
}
