#include "graph.h"

float getY(float R, float G, float B)
{
	float y;
	return y = 0.257*R + 0.564*G + 0.098*B + 16;
}
float getCb(float R, float G, float B)
{
	float y;
	return y = -0.148*R - 0.291*G + 0.439*B + 128;
}
float getCr(float R, float G, float B)
{
	float y;
	return y = 0.439*R - 0.368*G - 0.071*B + 128;
}

/**²âÊÔĞÅºÅ£¬5Ö¡£¬ ºÚ°×ºÚ°×ºÚ
·Ö±æÂÊ32*32
**/
//void main(void)
//{
//	using namespace std;
//	std::ofstream of2("E://graphLibrary//imgInfo_test.txt");
//	for (int i = 0; i < 5; i++)
//	{
//		switch (i%2)
//		{
//		case 0:
//			for (int j = 0; j < 32*32*3; j++)
//			{
//				of2 << "0" << endl;
//			}
//			break;
//		default:
//			for (int j = 0; j < 32 * 32 * 3; j++)
//			{
//				of2 << "12" << endl;
//			}
//		}
//	}
//	cout << "finished" << endl;
//	getchar();
//}

/** ²âÊÔĞÅºÅ2£¬1234512345£¬32*32**/
//void main(void)
//{
//	using namespace std;
//	int a = 0;
//	int mark = 1;
//	int cc = 0;
//	ofstream of3("E://graphLibrary//imgInfo_result.txt");
//		for (int i = 0; i < 32 * 32 * 3 * 5; i++)
//		{
//			if (mark == 17)
//			{
//				mark = 16;
//				cc = 1;
//			}
//			if (mark == 0)
//			{
//				mark = 1;
//				cc = 0;
//			}
//			if (cc == 0)
//			{
//				of3 << "@" << i<<" ";
//				of3 << mark << endl;
//				mark = mark + 1;
//			}
//			else
//			{
//				of3 << "@" << i << " ";
//				of3 << mark << endl;
//				mark = mark - 1;
//			}
//		}
//
//}

//²âÊÔĞÅºÅ£¬100-001£¬128*64*5*3
//void main(void)
//{
//	using namespace std;
//	ofstream of3("E://graphLibrary//test_signal.dat");
//	int a = 0;
//	for (int i = 0; i < 128 * 128 * 5; i++)
//	{
//		if(i%2 == 0)
//			of3 << "1" << endl << "1" << endl << "1" << endl;
//		else
//			of3 << "0" << endl << "0" << endl << "0" << endl;
//	}
//}

//void main(void)
//{
//	using namespace std;
//	int a = 0;
//	int mark = 1;
//	int cc = 0;
//	ofstream of3("E://graphLibrary//test_result.txt");
//	for (int i = 0; i < 16 * 16 * 3 * 5; i++)
//	{
//		if (mark == 17)
//		{
//			mark = 0;
//		}
//		of3 << mark << endl;
//		mark = mark + 1;
//	}
//
//}

////Write into file
//void main(void)
//{
//	graph graph1 = graph("E://graphLibrary//flower.jpg", 0);
//	graph1.text_to_graph();
//}
////²âÊÔĞÅºÅ£¬CbY1CrY2¸ñÊ½
//void main()
//{
//	int blue[4] = { int(getCb(0,0,205)),int(getY(0,0,205)),int(getCr(0,0,205)),int(getY(0,0,205)) };
//	int yellow[4] = { int(getCb(255,255,0)),int(getY(255,255,0)),int(getCr(255,255,0)),int(getY(255,255,0)) };
//	int purple[4] = {	int(getCb(218,112,214)),int(getY(218,112,214)),int(getCr(218,112,214)),int(getY(218,112,214)) };
//	std::ofstream of4("E://graphLibrary//YCrCb_test.dat");
//	for (int i = 0; i < 270000 /*×ÜÏñËØÊı*/; i++)
//	{
//		if (i % 3 == 0)
//		{
//			of4 << blue[0] << std::endl << blue[1] << std::endl << blue[2] << std::endl << blue[3] << std::endl;
//		}
//		else if (i % 3 == 1)
//		{
//			of4 << yellow[0] << std::endl << yellow[1] << std::endl << yellow[2] << std::endl << yellow[3] << std::endl;
//		}
//		else if (i % 3 == 2)
//		{
//			of4 << purple[0] << std::endl << purple[1] << std::endl << purple[2] << std::endl << purple[3] << std::endl;
//		}
//	}
//	of4.close();
//}
//Read from graph
//int main()
//{
//	graph a = graph("E://graphLibrary//flower.jpg", 0);
//	a.graph_to_text();
//	getchar();
//    return 0;
//}
////Generate mosaic graph
//void main(void)
//{
//	graph graph1 = graph("E://graphLibrary//tower.png", 0);
//	int ** a = graph1.toBayer("E://graphLibrary//tower_mosaic.jpg");
//	graph1.Bayer_demosaic("E://graphLibrary/tower_demosaic.jpg", a);
//}
//Bayer to text
//void main() 
//{
//	graph graph1 = graph("E://graphLibrary//tower.png", 0);
//	int **img_data = graph1.toBayer("E://graphLibrary//tower_mosaic.jpg");
//	graph graph2 = graph("E://graphLibrary//tower_mosaic.jpg", 0);
//	graph2.Bayer_to_text2("E://graphLibrary//tower_Bayer.dat", img_data);
//}
// 512 * 768 test signal , B: 0, G: 1, R : 2
void main(void)
{
	std::ofstream of1("E://graphLibrary//Bayer_test.txt");
	bool is_even = false;
	bool is_even_col = false;
	for (int i = 0; i < 768; i++) {
		for (int j = 0; j < 512; j++) {
			of1 << (is_even ? (is_even_col ? 2:1) : ((is_even_col ? 1:0))) << std::endl;
			is_even_col = !is_even_col;
		}
		is_even = !is_even;
		is_even_col = false;
	}
}
/**
2 1 2 1 2 1 2 1 2 1 2 1

1 0 1 0 1 0 1 0 1 0 1 0

2 1 2 1 2 1 2 1 2 1 2 1

1 0 1 0 1 0 1 0 1 0 1 0

2 1 2 1 2 1 2 1 2 1 2 1

1 0 1 0 1 0 1 0 1 0 1 0
**/