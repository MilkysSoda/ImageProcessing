#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "fn.h"
void main(void)
{
	char name[100];
	int width, height, ** orgimg, ** outimg;
	int histo[256];
	GetFileName(name, "Enter test image name : ");
	GetSize(&width, &height);
	orgimg = (int**)Imalloc2(width, height);
	outimg = (int**)Imalloc2(width, height);
	ReadImage(name, orgimg, width, height);
	
	ComputeHistogram(orgimg, histo, width, height);
	//Histogram을 분석하여 histo[256]에 값 저장
	DrawHistogram(histo, width, height);
	//저장한 값을 Visualize 하여 그림으로 저장
	
	HistogramEqualization(orgimg, outimg, histo, width, height); 
	//저장된 값을 Equlization 하여 Spectrum 분할

	ComputeHistogram(outimg, histo, width, height);
	//Equlize한 histo를 다시 Counting
	DrawHistogram(histo, width, height);
	//분할한 histo에 맞춰 다시 Visualize

	GetFileName(name, "Enter output image name : ");
	WriteImage(name, outimg, width, height);
	Ifree2(orgimg, width, height);
	Ifree2(outimg, width, height);
}

void HistogramEqualization(int** orgimg, int** outimg, int* histo, int width, int height)
{
	int i, j, color, sum = 0;
	double sumf = 0.;
	for (color = 0, sumf = 0.; color < 255; color++) {
		sumf += 255. * histo[color] / (width * height); // 
		histo[color] = (int)(sumf + .5); //반올림을 통하여 오류 방지
	}
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
			outimg[j][i] = histo[orgimg[j][i]];
}

void ComputeHistogram(int** IN, int* histo, int width, int height)
{
	int i, j;
	for (i = 0; i < 256; i++) histo[i] = 0;
	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++) {
			histo[IN[j][i]]++;
		}
}

void DrawHistogram(int* histo, int width, int height)
{
	int** hisimg, i, j, sum, color, max_histo;
	float sumf;
	char name[100];
	hisimg = (int**)Imalloc2(256, 256);
	for (j = 0; j < height; j++) for (i = 0; i < width; i++) hisimg[j][i] = 0;
	for (max_histo = 0, color = 0; color < 256; color++)
		if (histo[color] > max_histo) max_histo = histo[color];
	for (color = 0; color < 256; color++)
		for (j = height - histo[color] * height / (3 * max_histo); j < height; j++)
			hisimg[j][color] = 255;
	for (sum = 0, color = 0; color < 256; color++) {
		sum += histo[color];
		sumf = height - (float)sum / (width * height) * height;
		hisimg[Clipping((int)(sumf))][color] = (hisimg[Clipping((int)(sumf))][color] == 0) ? 255 : 0;
	}
	GetFileName(name, "Enter histogram image name : ");
	WriteImage(name, hisimg, 256, 256);
	Ifree2(hisimg, width, height);
}

void MakeMachBand(int** img, int W, int H)
{
	int i, j, k, no_bands = 8, band_width;

	band_width = (int)(W / no_bands);
	for (k = 0; k < no_bands; k++)
		for (i = 0; i < band_width; i++)
			for (j = 0; j < H; j++)
				img[j][k * band_width + i] = (k * band_width);
}

void GetFileName(char* name, char* string)
{
	printf("\n\t[] %s \t", string);
	scanf("%s", name);
}

void GetSize(int* width, int* height)
{
	printf("\n\t[] (width,height)\t");
	scanf("%d%d", width, height);
}

void ReadImage(char* name, int** image, int width, int height)
{
	FILE* fp;  int i, j;

	fp = fopen(name, "rb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		image[i][j] = getc(fp);
	fclose(fp);
}

void WriteImage(char* name, int** image, int width, int height)
{
	FILE* fp; int i, j;

	fp = fopen(name, "wb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		putc((unsigned char)image[i][j], fp);
	fclose(fp);
}

void WriteRGBImage(char* name, int** image_r, int** image_g, int** image_b, int width, int height)
{
	FILE* fp; int i, j;

	fp = fopen(name, "wb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		putc((unsigned char)image_r[i][j], fp);
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		putc((unsigned char)image_g[i][j], fp);
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		putc((unsigned char)image_b[i][j], fp);
	fclose(fp);
}

void ReadRGBImage(char* name, int** image_r, int** image_g, int** image_b, int width, int height)
{
	FILE* fp; int i, j;

	fp = fopen(name, "rb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		image_r[i][j] = getc(fp);
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		image_g[i][j] = getc(fp);
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		image_b[i][j] = getc(fp);
	fclose(fp);
}

void WriteYUVImage(char* name, int** image_y, int** image_u, int** image_v, int width, int height)
{
	FILE* fp; int i, j;

	fp = fopen(name, "wb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		putc((unsigned char)image_y[i][j], fp);
	for (i = 0;i < height / 2;i++) for (j = 0;j < width;j++) {
		if (j < width / 2)
			putc((unsigned char)image_u[i][j], fp);
		else
			putc((unsigned char)image_v[i][j - width / 2], fp);
	}
	fclose(fp);
}

void ReadYUVImage(char* name, int** image_y, int** image_u, int** image_v, int width, int height)
{
	FILE* fp; int i, j;

	fp = fopen(name, "rb");
	if (fp == NULL) { printf("\n\t[] Open Failure !!"); exit(0); }
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		image_y[i][j] = getc(fp);
	for (i = 0;i < height / 2;i++) for (j = 0;j < width;j++) {
		if (j < width / 2)
			image_u[i][j] = getc(fp);
		else
			image_v[i][j - width / 2] = getc(fp);
	}
	fclose(fp);
}

void ReadBlock(int** img, int** block, int a, int b, int wx, int wy, int width, int height)
{
	int i, j, x, y;

	for (j = 0; j < wy; j++)
		for (i = 0; i < wx; i++) {
			x = ((a + i) < width) ? a + i : width - 1;
			y = ((b + j) < height) ? b + j : height - 1;
			block[j][i] = img[y][x];
		}
}

void WriteBlock(int** img, int** block, int a, int b, int wx, int wy, int width, int height)
{
	int i, j, x, y;

	for (j = 0; j < wy; j++)
		for (i = 0; i < wx; i++) {
			x = ((a + i) < width) ? a + i : width - 1;
			y = ((b + j) < height) ? b + j : height - 1;
			img[y][x] = block[j][i];
		}
}

int** Imalloc2(int x, int y)
{
	int i; int** tmp;

	tmp = (int**)malloc(sizeof(int*) * y);
	for (i = 0;i < y;i++) tmp[i] = (int*)malloc(sizeof(int) * x);
	return(tmp);
}

void Ifree2(int** array, int x, int y)
{
	int i;
	for (i = 0;i < y;i++) free(array[i]);
	free(array);
}

float** Fmalloc2(int x, int y)
{
	int i; float** tmp;

	tmp = (float**)malloc(sizeof(float*) * y);
	for (i = 0;i < y;i++) tmp[i] = (float*)malloc(sizeof(float) * x);
	return(tmp);
}

void Ffree2(float** array, int x, int y)
{
	int i;
	for (i = 0;i < y;i++) free(array[i]);
	free(array);
}


void CopyImage(int** IN, int** OUT, int width, int height)
{
	int i, j;
	for (i = 0;i < height;i++) for (j = 0;j < width;j++) OUT[i][j] = IN[i][j];
}

int Clipping(int value)
{
	if (value < 0) return(0);
	else if (value > 255) return(255);
	else return(value);
}

void MakeDiffImage(int** image1, int** image2, int** imageout, int width, int height)
{
	int i, j;
	for (i = 0;i < height;i++) for (j = 0;j < width;j++)
		imageout[i][j] = abs(image1[i][j] - image2[i][j]);
}

void MakeZonePlate(int** img, int W, int H)
{
	int i, j;
	double alpha = 0.007067, beta = 0.007068;

	for (j = 0; j < H; j++) {
		for (i = 0; i < W; i++) {
			img[j][i] = (int)(127. * cos(alpha * (i - W / 2.) * (i - W / 2.) + beta * (j - H / 2.) * (j - H / 2.)) + 127.);
		}
	}
}
