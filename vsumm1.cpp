#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>
#include <cmath>
using namespace std;
using namespace cv;
double scravgnew=0,scravgold=0,scrvar=0,scrsd=0,cnt=0;
double sc;
Mat a[10];
int b[10];
long int modulo(long int a)
{
    if(a>0)
        return a;
    else
        return(-1*a);
}
void score(Mat f2,Mat f1,int t)
{

/*int cn1=f1.channels(),cn2=f2.channels();
Scalar_<uint8_t> p1,p2;

for(int i=0;i<f2.rows;i++)
{
    uint8_t* rowPtr1=f1.row(i);
    uint8_t* rowPtr2=f2.row(i);
    for(int j=0;j<f2.cols;j++)
   {
p1.val[0]=rowPtr1[j*cn1+0];
p1.val[1]=rowPtr1[j*cn1+1];
p1.val[2]=rowPtr1[j*cn1+2];
p2.val[0]=rowPtr2[j*cn2+0];
p2.val[1]=rowPtr2[j*cn2+1];
p2.val[2]=rowPtr2[j*cn2+2];
sc+=(p1.val[0]-p2.val[0])*(p1.val[0]-p2.val[0])+(p1.val[1]-p2.val[1])*(p1.val[1]-p2.val[1])+(p1.val[2]-p2.val[2])*(p1.val[2]-p2.val[2]);
   }
}*/
   int r1,r2,g1,g2,b1,b2;
   Mat m1;
   sc=0;
   absdiff(f1,f2,m1);
    for(int i=0;i<f2.rows;i++)
    {
        for(int j=0;j<f2.cols;j++)
       {
        b1=f1.at<Vec3b>(i,j)[0];
        g1=f1.at<Vec3b>(i,j)[1];
        r1=f1.at<Vec3b>(i,j)[2];
        b2=f2.at<Vec3b>(i,j)[0];
        g2=f2.at<Vec3b>(i,j)[1];
        r2=f2.at<Vec3b>(i,j)[2];
        sc+=modulo(r1-r2)+modulo(g1-g2)+modulo(b1-b2);
        //sc+=m1.at<Vec3b>(i,j)[1]+m1.at<Vec3b>(i,j)[2]+m1.at<Vec3b>(i,j)[3];
        }
    }
    sc=sc/(3*f2.rows*f2.cols);
     //sc[cnt]=sum(m1)[0]+sum(m1)[1]+sum(m1)[2];
     if(t==1)
     {
     scravgnew=(scravgold*cnt+sc)/(cnt+1);
     scrvar=(scrvar*cnt+cnt*scravgold*scravgold-(cnt+1)*scravgnew*scravgnew+sc*sc)/(cnt+1);
     cnt++;
     scravgold=scravgnew;
     scrsd=sqrt(scrvar);
     }
}
int main(int argc, char **argv)
{
    char q;Mat frame,prevframe,f1;
    VideoCapture cap(argv[1]); // open the default camera
    //cout<<argv[1];
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    int i=0;
    Mat edges;

    //namedWindow("edges",1);
    cap.read(frame);
    resize(frame,f1,Size(500,500*frame.rows/frame.cols));
    f1.copyTo(prevframe);
    //frame.copyTo(f1);

    string text;
    VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),30, Size(f1.cols,f1.rows),true);
    while(cap.read(frame))//cap.read(frame))
    {
        //cvtColor(frame,frame,CV_RGB2GRAY);
        //cap >> frame; // get a new frame from camera
        //q=waitKey(20);
        //cout<<"Check";
        resize(frame,f1,Size(500,500*frame.rows/frame.cols));
        score(prevframe,f1,1);
        /*stringstream ss;
        ss << scr;
        text = ss.str();
        //cout<<text<<endl;
        frame.copyTo(f1);
        putText(f1, text, Point(40,40), FONT_HERSHEY_PLAIN, 4, Scalar(0,100,100,100),3,8,0);
        imshow("blah",f1);*/
        f1.copyTo(prevframe);
                //waitKey(30);       /*waitKey(1);*/
    }
cap.release();
cap.open(argv[1]);
waitKey(1000);
cap.read(frame);
    resize(frame,f1,Size(500,500*frame.rows/frame.cols));
    f1.copyTo(prevframe);
 //cout<<scravgnew<<" "<<scrsd;
   for(i=0;i<10;i++)
    {a[i]=Mat::zeros(f1.rows,f1.cols,CV_8UC1);
b[i]=0;
}
 i=0;
while(i<10&&cap.read(frame))
{
        resize(frame,f1,Size(500,500*frame.rows/frame.cols));
    f1.copyTo(a[i]);
    i++;
}
while(cap.read(frame))
{
resize(frame,f1,Size(500,500*frame.rows/frame.cols));

for(i=0;i<9;i++)
{
    a[i+1].copyTo(a[i]);
b[i]=b[i+1];
}
f1.copyTo(a[9]);
        score(a[5],a[6],0);
        //frame.copyTo(prevframe);
        if(sc>scravgnew+scrsd/2)
            {
                for(i=0;i<9;i++)
{
    if(b[i]==0)
    video.write(a[i]);
b[i]=1;
}


            }
    }
VideoCapture output("out.avi");
if (!output.isOpened())
{
        std::cout << "!!! Output video could not be opened" << std::endl;
        return 0;
}
while(output.read(f1))
    {
    imshow("blah",f1);
    waitKey(30);
    }

    waitKey(0);  // the camera will be deinitialized automatically in VideoCapture destructor
cap.release();
video.release();
output.release();
    return 0;
}
