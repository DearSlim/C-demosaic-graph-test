#include "graph.h"
using namespace std;
using namespace cv;
const int rows = 768;
const int cols = 512;
const int total_length = 128 * 128;

int avg(int a, int b)
{
	return (a + b) / 2;
}
int avg(int a, int b, int c, int d)
{
	return (a + b + c + d) / 4;
}
int avoidOverflow(int a)
{
	int b = 0;
	if (a > 0)
		return b = a > 255 ? 255 : a;
	else
		return b = 0;
}

graph::graph(string fpath, int mode)
{
	this->fpath = fpath;
	switch (mode)
	{
	case 0: this->mode = read; break;
	case 1: this->mode = write; break;
	//case 2: this->mode = readwrite; break;
	default:  cout<<"mode value invaild."<<endl;
	}
}

Mat * graph::graph_to_text()
{	
	Mat img_in = imread(this->fpath, 1);
	ofstream of("E://graphLibrary//flower.dat");
	if (of.fail())
	{
		cout << "open ofstream failed.\n";
		exit(0);
	}
	char temp[7];
	int temp1;
	for (int i = 0; i < img_in.rows; i++)
	{
		for (int j = 0; j < img_in.cols; j++)
		{
			temp1 = (int)img_in.at<Vec3b>(i, j)[0];
			_itoa_s(temp1, temp, 16);
			of << temp;
			of << endl;

			temp1 = (int)img_in.at<Vec3b>(i, j)[1];
			_itoa_s(temp1, temp, 16);
			of << temp;
			of << endl;

			temp1 = (int)img_in.at<Vec3b>(i, j)[2];
			_itoa_s(temp1, temp, 16);
			of << temp;
			of << endl;
		}
	}
	cout << "finished" << endl;
	of.close();
	return &img_in;
}

int graph::text_to_graph()
{
	int temp;
	vector <int> value;
	int m = 0;
	ifstream if1("E://graphLibrary//filtered_result2.dat");
	Mat_<Vec3b> img_out(64, 64);
	
	for (int i = 0; i < 64 * 64 * 3 * 5; i++)
	{
		if1 >> temp;
		value.push_back(temp);
	}
	for (int n = 0; n < 5; n++)
	{
		for (int i = 0; i < 64; i++)
		{
			for (int j = 0; j < 64; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					img_out.at<Vec3b>(i, j)[k] = value[m];
					m++;
				}
			}
		}
		String filepath = "E://graphLibrary//frame";
		filepath += (n + 1 + '0');
		filepath += ".jpg";
		cv::imwrite(filepath,img_out);
	}
	return 0;
}

int graph::toGray(void)
{
	Mat img_in = imread(this->fpath, 1);
	Mat_<Vec3b> img_out(128, 128);
	float a = 0;
	for (int i = 0; i < img_in.rows; i++)
	{
		for (int j = 0; j < img_in.cols; j++)
		{
			a = img_in.at<Vec3b>(i, j)[0] * 0.114 +
				img_in.at<Vec3b>(i, j)[1] * 0.587 +
				img_in.at<Vec3b>(i, j)[2] * 0.3;
			img_out.at<Vec3b>(i, j)[0] = img_out.at<Vec3b>(i, j)[1] = img_out.at<Vec3b>(i, j)[2] = a;
		}
	}
	String filepath = "E://graphLibrary//grayGraph.jpg";
	imwrite(filepath, img_out);
	return 0;
}
//Generate Bayer sequence of a full color image for testing demosaic algorithms
int graph::mosaic(int level, String opath)
{
	Mat img_in = imread(this->fpath, 1);
	Mat_<Vec3b> img_out(img_in.rows, img_in.cols);
	default_random_engine e;
	uniform_int_distribution<unsigned> u(0, 1);
	int m = 0, n = 0;
	for (int i = 0; i < img_in.rows; i=i+sqrt(level))
	{
		n = 0;
		for (int j = 0; j < img_in.cols; j=j+sqrt(level))
		{
			img_out.at<Vec3b>(m, n) = img_in.at<Vec3b>(i + u(e), j + u(e));
			for (int k = 0; k < sqrt(level); k++)
			{
				if (m+k < img_in.rows)
				{
					for (int o = 0; o < sqrt(level); o++)
					{
						if (n+o < img_in.cols)
						{
							img_out.at<Vec3b>(m + k, n + o) = img_out.at<Vec3b>(m, n);
						}
						else {
							break;
						}
					}
				}
				else {
					break;
				}
			}
			n += sqrtf(level);
		}
		m += sqrtf(level);
	}
	imwrite(opath, img_out);
	return 0;
}
//
int ** graph::toBayer(string opath)
{
	Mat img_in = imread(this->fpath, 1);
	Mat_<Vec3b> img_out(img_in.rows, img_in.cols);
	int **img_data = new int*[img_in.rows];
	for (int i = 0; i < img_in.rows; i++) {
		img_data[i] = new int[img_in.cols];
	}
	cout << img_in.rows << "    " << img_in.cols << endl;
	bool is_even = false;
	int BGR = 2;  //Distinguish which should be output.0B1G2R
	for (int i = 0; i < img_in.rows; i++) {
		for (int j = 0; j < img_in.cols; j++){
			if (is_even) {
				switch (BGR)
				{
				case 1:
					img_out.at<Vec3b>(i, j)[0] = 0;
					img_out.at<Vec3b>(i, j)[1] = img_in.at<Vec3b>(i, j)[1];
					img_data[i][j] = img_in.at<Vec3b>(i, j)[1];
					img_out.at<Vec3b>(i, j)[2] = 0;
					BGR = 0;
					break;
				case 0:
					img_out.at<Vec3b>(i, j)[0] = img_in.at<Vec3b>(i, j)[0];
					img_data[i][j] = img_in.at<Vec3b>(i, j)[0];
					img_out.at<Vec3b>(i, j)[1] = 0;
					img_out.at<Vec3b>(i, j)[2] = 0;
					BGR = 1;
					break;
				default:
					break;
				}
			}
			else {
				switch (BGR)
				{
				case 2:
					img_out.at<Vec3b>(i, j)[0] = 0;
					img_out.at<Vec3b>(i, j)[1] = 0;
					img_out.at<Vec3b>(i, j)[2] = img_in.at<Vec3b>(i,j)[2];
					img_data[i][j] = img_in.at<Vec3b>(i, j)[2];
					BGR = 1;
					break;
				case 1:
					img_out.at<Vec3b>(i, j)[0] = 0;
					img_out.at<Vec3b>(i, j)[1] = img_in.at<Vec3b>(i, j)[1];
					img_data[i][j] = img_in.at<Vec3b>(i, j)[1];
					img_out.at<Vec3b>(i, j)[2] = 0;
					BGR = 2;
					break;
				default:
					break;
				}
			}
		}
		BGR = is_even ? 2 : 1;
		is_even = !is_even;
	}
	imwrite(opath, img_out);
	return img_data;
}

int graph::Bayer_demosaic(string opath, int **img_data)
{
	Mat_<Vec3b> img_out(rows, cols);
	int pstate = 0;
	enum BGR { B = 0, G = 1, R = 2 } BGR = R;
	//int BGR = 2; //0B1G2R
	bool is_even = false;
	int updata, leftdata, downdata, rightdata, ul, ur, dl, dr;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//求出插值需要的数据。边角的地方空余的数值用和旁边相等的值代替，不会影响计算结果
			if (i == 0) {
				updata = downdata = img_data[i + 1][j];
				if (j == 0) {
					ul = ur = dl = dr = img_data[i + 1][ j + 1];
				}
				else if (j == cols-1) {
					ul = ur = dr = dl = img_data[i + 1][j - 1];
				}
				else {
					ul = dl = img_data[i + 1][j - 1];
					ur = dr = img_data[i + 1][j + 1];
				}
			}
			else if (i==rows-1) {
				downdata = updata = img_data[i - 1][j];
				if (j == 0) {
					ul = dr = dl = ur = img_data[i - 1][j + 1];
				}
				else if (j == cols-1) {
					ul = dl = dr = ul = img_data[i - 1][j - 1];
				}
				else {
					dl = ul = img_data[i - 1][j - 1];
					dr = ur = img_data[i - 1][j + 1];
				}
			}
			else {
				updata = img_data[i - 1][j];
				downdata = img_data[i + 1][j];
				if (j == 0) {
					ul = ur = img_data[i - 1][j + 1];
					dl = dr = img_data[i + 1][j + 1];
				}
				else if (j == cols-1) {
					ur = ul = img_data[i - 1][j - 1];
					dr = dl = img_data[i + 1][j - 1];
				}
				else {
					ul = img_data[i - 1][j - 1];
					ur = img_data[i - 1][j + 1];
					dl = img_data[i + 1][j - 1];
					dr = img_data[i + 1][j + 1];
				}
			}
			if (j == 0) {
				leftdata = rightdata = img_data[i][j + 1];
			}
			else if (j == cols-1) {
				rightdata = leftdata = img_data[i][j - 1];
			}
			else {
				leftdata = img_data[i][j - 1];
				rightdata = img_data[i][j + 1];
			}
			//开始插值
			if (is_even) {
				switch (BGR) {
				case G:
					img_out.at<Vec3b>(i, j)[0] = avg(leftdata,rightdata);
					img_out.at<Vec3b>(i, j)[1] = img_data[i][j];
					img_out.at<Vec3b>(i, j)[2] = avg(updata, downdata);
					BGR = B;
					break;
				case B:
					img_out.at<Vec3b>(i, j)[0] = img_data[i][j];
					img_out.at<Vec3b>(i, j)[1] = abs(leftdata-rightdata)<abs(updata-downdata) ? avg(leftdata, rightdata):avg(updata,downdata);
					//img_out.at<Vec3b>(i, j)[1] = avg(updata, downdata,leftdata, rightdata);
					img_out.at<Vec3b>(i, j)[2] = avg(ul, ur, dl, dr);
					BGR = G;
					break;
				default:
					cout << "Even BGR error" << endl;
					cout << BGR << endl;
					break;
				}
			}
			else {
				switch (BGR) {
				case R:
					img_out.at<Vec3b>(i, j)[0] = avg(ul, ur, dl, dr);
					//img_out.at<Vec3b>(i, j)[1] = avg(updata, downdata, leftdata, rightdata);
					img_out.at<Vec3b>(i, j)[1] = abs(leftdata - rightdata)<abs(updata - downdata) ? avg(leftdata, rightdata) : avg(updata, downdata);
					img_out.at<Vec3b>(i, j)[2] = img_data[i][j];
					BGR = G;
					break;
				case G:
					img_out.at<Vec3b>(i, j)[0] = avg(updata, downdata);
					img_out.at<Vec3b>(i, j)[1] = img_data[i][j];
					img_out.at<Vec3b>(i, j)[2] = avg(leftdata,rightdata);
					BGR = R;
					break;
				default:
					cout << "Odd BGR error" << endl;
					break;
				}
			}

		}
		BGR = is_even ? R : G;
		is_even = !is_even;
	}
	imwrite(opath, img_out);
	return 0;
}

int graph::Bayer_demosaic_smooth(string opath, int **img_data)
{
	//cout << rows << endl << cols;
	bool is_even = false;
	Mat_<Vec3b> img_out(rows, cols);
	enum BGR { B = 0, G = 1, R = 2 } BGR = R;
	int updata, leftdata, downdata, rightdata, ul, ur, dl, dr;
	updata = 0;
	downdata = 0;
	int uu = 0;
	int dd = 0;
	int ll = 0;
	int rr = 0;
	int G_temp = 0;
	int dH = 0;
	int dV = 0;
	int D1 = 0;
	int D2 = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//求出插值需要的数据。边角的地方空余的数值用和旁边相等的值代替，不会影响计算结果
			//计算ul,ur,dl,dr
			if (i == 0) {
				updata = downdata = img_data[i + 1][j];
				if (j == 0) {
					ul = ur = dl = dr = img_data[i + 1][j + 1];
				}
				else if (j == cols - 1) {
					ul = ur = dr = dl = img_data[i + 1][j - 1];
				}
				else {
					ul = dl = img_data[i + 1][j - 1];
					ur = dr = img_data[i + 1][j + 1];
				}
			}
			else if (i == rows - 1) {
				downdata = updata = img_data[i - 1][j];
				if (j == 0) {
					ul = dr = dl = ur = img_data[i - 1][j + 1];
				}
				else if (j == cols - 1) {
					ul = dl = dr = ul = img_data[i - 1][j - 1];
				}
				else {
					dl = ul = img_data[i - 1][j - 1];
					dr = ur = img_data[i - 1][j + 1];
				}
			}
			else {
				updata = img_data[i - 1][j];
				downdata = img_data[i + 1][j];
				if (j == 0) {
					ul = ur = img_data[i - 1][j + 1];
					dl = dr = img_data[i + 1][j + 1];
				}
				else if (j == cols - 1) {
					ur = ul = img_data[i - 1][j - 1];
					dr = dl = img_data[i + 1][j - 1];
				}
				else {
					ul = img_data[i - 1][j - 1];
					ur = img_data[i - 1][j + 1];
					dl = img_data[i + 1][j - 1];
					dr = img_data[i + 1][j + 1];
				}
			}
			//计算uu,dd
			if (i == 0 || i==1) {
				uu = dd = img_data[i + 2][j];
			}
			else if (i == rows - 1 || i == rows - 2) {
				dd = uu = img_data[i - 2][j];
			}
			else {
				uu = img_data[i - 2][j];
				dd = img_data[i + 2][j];
			}
			//计算ll, rr
			if (j == 0 || j == 1) {
				ll = rr = img_data[i][j + 2];
			}
			else if (j == cols - 1 || j == cols - 2) {
				rr = ll = img_data[i][j - 2];
			}
			else {
				ll = img_data[i][j - 2];
				rr = img_data[i][j + 2];
			}
			//计算left,right,up,down
			if (j == 0) {
				leftdata = rightdata = img_data[i][j + 1];
			}
			else if (j == cols - 1) {
				rightdata = leftdata = img_data[i][j - 1];
			}
			else {
				leftdata = img_data[i][j - 1];
				rightdata = img_data[i][j + 1];
			}
			//汉密尔顿插值
			if (BGR == B || BGR == R) {
				dH = abs(leftdata - rightdata) + abs(2 * img_data[i][j] - ll - rr);
				dV = abs(updata - downdata) + abs(2 * img_data[i][j] - uu - dd);
				G_temp = dH > dV ? (updata + downdata) / 2 + (2 * img_data[i][j] - uu - dd) / 4 : (leftdata + rightdata) / 2 + (2 * img_data[i][j] - ll - rr) / 4;
				img_out.at<Vec3b>(i, j)[1] = G_temp = avoidOverflow(G_temp);
				D1 = abs(ur - dl) + abs(2 * G_temp - (updata + rightdata) / 2 - (downdata + leftdata) / 2);
				D2 = abs(ul - dr) + abs(2 * G_temp - (updata + leftdata) / 2 - (downdata + rightdata) / 2);
				int tempa = 2 * G_temp - (updata + rightdata) / 2 - (downdata + leftdata) / 2;
				int tempb = 2 * G_temp - (updata + leftdata) / 2 - (downdata + rightdata) / 2;
				int temp = avoidOverflow(D1 < D2 ? (ur + dl) / 2 + tempa / 2 : (ul + dr) / 2 + tempb / 2);
				img_out.at<Vec3b>(i, j)[0] = (BGR == B) ? img_data[i][j] : temp;
				img_out.at<Vec3b>(i, j)[2] = (BGR == R) ? img_data[i][j] : temp;
				BGR = G;
			}
			else {
				int a = avoidOverflow(avg(leftdata, rightdata) + (2 * img_data[i][j] - (ul + dl) / 2 - (ur + dr) / 2) / 2);
				int b = avoidOverflow(avg(updata, downdata) + (2 * img_data[i][j] - (ur + ul) / 2 - (dr + dl) / 2) / 2);
				img_out.at<Vec3b>(i, j)[0] = is_even ? a : b;
				img_out.at<Vec3b>(i, j)[1] = img_data[i][j];
				img_out.at<Vec3b>(i, j)[2] = is_even ? b : a;
				BGR = is_even ? B : R;
			}
		}
		BGR = is_even ? R : G;
		is_even = !is_even;
	}
	imwrite(opath, img_out);
	return 0;
}

int graph::Bayer_demosaic_smooth2(string opath, int **img_data)
{
	bool is_even = false;
	Mat_<Vec3b> img_out(rows, cols);
	enum BGR { B = 0, G = 1, R = 2 } BGR = R;
	int updata, leftdata, downdata, rightdata, ul, ur, dl, dr;
	int leftG, rightG, upG, downG;
	int Gur, Gul, Gdl, Gdr;
	int uu = 0;
	int dd = 0;
	int ll = 0;
	int rr = 0;
	int G_temp = 0;
	int dH = 0;
	int dV = 0;
	int D1 = 0;
	int D2 = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//计算uu,dd
			if (i == 0 || i == 1) {
				uu = dd = img_data[i + 2][j];
			}
			else if (i == rows - 1 || i == rows - 2) {
				dd = uu = img_data[i - 2][j];
			}
			else {
				uu = img_data[i - 2][j];
				dd = img_data[i + 2][j];
			}
			//计算ll, rr
			if (j == 0 || j == 1) {
				ll = rr = img_data[i][j + 2];
			}
			else if (j == cols - 1 || j == cols - 2) {
				rr = ll = img_data[i][j - 2];
			}
			else {
				ll = img_data[i][j - 2];
				rr = img_data[i][j + 2];
			}
			//计算left,right,up,down
			if (j == 0) {
				leftdata = rightdata = img_data[i][j + 1];
			}
			else if (j == cols - 1) {
				rightdata = leftdata = img_data[i][j - 1];
			}
			else {
				leftdata = img_data[i][j - 1];
				rightdata = img_data[i][j + 1];
			}
			if (i == 0) {
				updata = downdata = img_data[i + 1][j];
			}
			else if(i == rows - 1) {
				downdata = updata = img_data[i - 1][j];
			}
			else {
				updata = img_data[i - 1][j];
				downdata = img_data[i + 1][j];
			}
			//计算RB的G值
			if (BGR == B || BGR == R) {
				dH = abs(leftdata - rightdata) + abs(2 * img_data[i][j] - ll - rr);
				dV = abs(updata - downdata) + abs(2 * img_data[i][j] - uu - dd);
				if (dH == dV) {
					img_out.at<Vec3b>(i, j)[1] = avoidOverflow((updata + downdata + rightdata + leftdata) / 4 + (4 * img_data[i][j] - ll - rr - uu - dd)/8);
				}
				else {
					img_out.at<Vec3b>(i, j)[1] = dH > dV ? avoidOverflow((updata + downdata) / 2 + (2 * img_data[i][j] - uu - dd) / 4) : 
						avoidOverflow((leftdata + rightdata) / 2 + (2 * img_data[i][j] - ll - rr) / 4);
				}
				BGR = G;
			}
			else {
				img_out.at<Vec3b>(i, j)[1] = img_data[i][j];
				BGR = is_even ? B : R;
			}
		}
		BGR = is_even ? R : G;
		is_even = !is_even;
	}
	//初始化
	BGR = R;
	is_even = false;
	//计算G的RB值
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//求出插值需要的数据。边角的地方空余的数值用和旁边相等的值代替
			//计算ul,ur,dl,dr,四方向的G分量
			if (i == 0) {
				updata = downdata = img_data[i + 1][j];
				upG = downG = img_out.at<Vec3b>(i + 1, j)[1];

			}
			else if (i == rows - 1) {
				downdata = updata = img_data[i - 1][j];
				downG = upG = img_out.at<Vec3b>(i - 1, j)[1];
			}
			else {
				updata = img_data[i - 1][j];
				downdata = img_data[i + 1][j];
				upG = img_out.at<Vec3b>(i - 1, j)[1];
				downG = img_out.at<Vec3b>(i - 1, j)[1];
			}
			//计算left,right
			if (j == 0) {
				leftdata = rightdata = img_data[i][j + 1];
				leftG = rightG = img_out.at<Vec3b>(i, j + 1)[1];
			}
			else if (j == cols - 1) {
				rightdata = leftdata = img_data[i][j - 1];
				rightG = leftG = img_out.at<Vec3b>(i, j - 1)[1];
			}
			else {
				leftdata = img_data[i][j - 1];
				rightdata = img_data[i][j + 1];
				leftG = img_out.at<Vec3b>(i, j - 1)[1];
				rightG = img_out.at<Vec3b>(i, j + 1)[1];
			}
			//为G插入R和B
			if (BGR == B || BGR == R) {
				BGR = G;
			}
			else {
				int a = avoidOverflow(avg(leftdata, rightdata) + (2 * img_data[i][j] - leftG - rightG) / 2);
				int b = avoidOverflow(avg(updata, downdata) + (2 * img_data[i][j] - upG - downG) / 2);
				img_out.at<Vec3b>(i, j)[0] = is_even ? a : b;
				img_out.at<Vec3b>(i, j)[2] = is_even ? b : a;
				BGR = is_even ? B : R;
			}
		}
		BGR = is_even ? R : G;
		is_even = !is_even;
	}
	//初始化
	BGR = R;
	is_even = false;
	//计算RB剩余值
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//计算ul,ur,dl,dr
			if (i == 0) {
				updata = downdata = img_data[i + 1][j];
				if (j == 0) {
					Gul = Gdl = Gur = Gdr = img_out.at<Vec3b>(i + 1, j + 1)[1];
					ul = ur = dl = dr = img_data[i + 1][j + 1];
				}
				else if (j == cols - 1) {
					Gul = Gur = Gdr = Gdl = img_out.at<Vec3b>(i + 1, j - 1)[1];
					ul = ur = dr = dl = img_data[i + 1][j - 1];
				}
				else {
					Gul = Gdl = img_out.at<Vec3b>(i + 1, j - 1)[1];
					Gur = Gdr = img_out.at<Vec3b>(i + 1, j + 1)[1];
					ul = dl = img_data[i + 1][j - 1];
					ur = dr = img_data[i + 1][j + 1];
				}
			}
			else if (i == rows - 1) {
				downdata = updata = img_data[i - 1][j];
				if (j == 0) {
					Gdl = Gdr = Gul = Gur = img_out.at<Vec3b>(i - 1, j + 1)[1];
					ul = dr = dl = ur = img_data[i - 1][j + 1];
				}
				else if (j == cols - 1) {
					Gdl = Gdr = Gur = Gul = img_out.at<Vec3b>(i - 1, j - 1)[1];
					ul = dl = dr = ul = img_data[i - 1][j - 1];
				}
				else {
					Gdl = Gul = img_out.at<Vec3b>(i - 1, j - 1)[1];
					Gdr = Gur = img_out.at<Vec3b>(i - 1, j + 1)[1];
					dl = ul = img_data[i - 1][j - 1];
					dr = ur = img_data[i - 1][j + 1];
				}
			}
			else {
				updata = img_data[i - 1][j];
				downdata = img_data[i + 1][j];
				if (j == 0) {
					Gul = Gur = img_out.at<Vec3b>(i - 1, j + 1)[1];
					Gdl = Gdr = img_out.at<Vec3b>(i + 1, j + 1)[1];
					ul = ur = img_data[i - 1][j + 1];
					dl = dr = img_data[i + 1][j + 1];
				}
				else if (j == cols - 1) {
					Gur = Gul = img_out.at<Vec3b>(i - 1, j - 1)[1];
					Gdr = Gdl = img_out.at<Vec3b>(i + 1, j - 1)[1];
					ur = ul = img_data[i - 1][j - 1];
					dr = dl = img_data[i + 1][j - 1];
				}
				else {
					Gul = img_out.at<Vec3b>(i - 1, j - 1)[1];
					Gur = img_out.at<Vec3b>(i - 1, j + 1)[1];
					Gdl = img_out.at<Vec3b>(i + 1, j - 1)[1];
					Gdr = img_out.at<Vec3b>(i + 1, j + 1)[1];
					ul = img_data[i - 1][j - 1];
					ur = img_data[i - 1][j + 1];
					dl = img_data[i + 1][j - 1];
					dr = img_data[i + 1][j + 1];
				}
			}
			//剩余RB插值
			D1 = abs(ur - dl) + abs(2 * img_out.at<Vec3b>(i, j)[1] - Gur - Gdl);
			D2 = abs(ul - dr) + abs(2 * img_out.at<Vec3b>(i, j)[1] - Gul - Gdr);
			if (BGR == B || BGR == R) {
				int tempa = 2 * img_out.at<Vec3b>(i, j)[1] - Gur - Gdl;
				int tempb = 2 * img_out.at<Vec3b>(i, j)[1] - Gul - Gdr;
				int temp = 0;;
				if (D1 == D2) {
					temp = (ur + ul + dr + dl) / 4 + (tempa + tempb) / 4;
				}
				else {
					temp = D1 < D2 ? (ur + dl) / 2 + tempa / 2 : (ul + dr) / 2 + tempb / 2;
				}
				temp = avoidOverflow(temp);
				img_out.at<Vec3b>(i, j)[0] = (BGR == B) ? img_data[i][j] : temp;
				img_out.at<Vec3b>(i, j)[2] = (BGR == R) ? img_data[i][j] : temp;
				BGR = G;
			}
			else {
				BGR = is_even ? B : R;
			}
		}
		BGR = is_even ? R : G;
		is_even = !is_even;
	}
	imwrite(opath, img_out);
	return 0;
}

int graph::Bayer_to_text(string opath) {
	Mat img_in = imread(this -> fpath, 1);
	ofstream of(opath);
	bool is_even = false;
	bool is_G = false;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			of << int(is_even ? (is_G ? img_in.at<Vec3b>(i, j)[1] : img_in.at<Vec3b>(i, j)[0]) :( is_G ? img_in.at<Vec3b>(i, j)[1] : img_in.at<Vec3b>(i, j)[2])) << endl;
			is_G = !is_G;
		}
		is_even = !is_even;
		is_G = !is_G;
	}
	return 0;
}

int graph::Bayer_to_text2(string opath, int **img_data) {
	ofstream of(opath);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			of << img_data[i][j] << endl;
		}
	}
	return 0;
}