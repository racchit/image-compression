#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // resize()
 
 
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
 
void createAlphaMat(Mat &mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}

int main(int argc, const char *argv[]){
    Mat mat = imread("imgs/input.png",-1);//CV_LOAD_IMAGE_ANYDEPTH);// the big pic
    // createAlphaMat(mat);

    vector<int> compression_params;
    vector<uchar> buf;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    try {
        imencode(".png", mat, buf, compression_params);
    }
    catch (cv::Exception& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
        return 1;
    }

    fprintf(stdout, "Saved PNG file with alpha data.\n");
    // for (int i = 0; i < buf.size(); i++) {
    //     cout << buf.at(i);
    // }

    ofstream OutputFile("output.txt");

    for (int i = 0; i < buf.size(); i++) {
        OutputFile << buf.at(i);
    }

    OutputFile.close();

    Mat out = imdecode(buf, IMREAD_ANYCOLOR);
    imwrite("final.png", out);
    return 0;
}