#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
#include <random>
using std::string;
class graph
{
public:
	graph(string fpath, int mode);
	cv::Mat * graph_to_text();
	int text_to_graph();
	int toGray();
	int mosaic(int level,string opath);
	int ** toBayer(string opath);
	int Bayer_demosaic(string fpath, int **img_data);
	int Bayer_demosaic_smooth(string fpath, int **img_data);
	int Bayer_demosaic_smooth2(string opath, int **img_data);
	int Bayer_to_text(string opath);
	int Bayer_to_text2(string opath, int **img_data);
private:
	std::string fpath;
	enum mode {read, write,readwrite} mode;
};


