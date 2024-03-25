#define SQ(x) ((x)*(x))
#define PI 3.141592
typedef struct {
       double im,re;
} complex;
#define clip(x) ((x<0) ? 0 : ((x>255) ? 255 : x))
int Clipping(int value);
void GetFileName(char *name,char *string);
void GetSize(int *width,int *height);
void ReadImage(char *name,int **image,int width,int height);
void WriteImage(char *name,int **image,int width,int height);
int **Imalloc2(int x,int y);
void Ifree2(int **array, int x, int y);
float **Fmalloc2(int x,int y);
void ReadBlock(int **img, int **block, int a, int b, int x, int y, int width, int height);
void CopyImage(int **IN, int **OUT, int width, int height);
void MakeDiffImage(int **image1, int **image2, int **imageout, int width, int height);
void MakeZonePlate(int **outimg, int width, int height);
void WriteRGBImage(char *name,int **image_r,int **image_g,int **image_b,int width,int height);
void MakeColorBar(int **img_r, int **img_g, int **img_b, int width, int height);
void ReadRGBImage(char *name,int **image_r,int **image_g,int **image_b,int width,int height);
void YUV2RGB(int **img_y, int **img_u, int **img_v, int **img_r, int **img_g, int **img_b, int width, int height);
void RGB2YUV(int **img_r, int **img_g, int **img_b, int **img_y, int **img_u, int **img_v, int width, int height);
void RGBPlaneSeperation(int **img_r, int **img_g, int **img_b, int width, int height);
void WriteYUVImage(char *name,int **image_y,int **image_u,int **image_v,int width,int height);
void ReadYUVImage(char *name,int **image_y,int **image_u,int **image_v,int width,int height);
void Ffree2(float **array, int x, int y);
void WriteBlock(int **img, int **block, int a, int b, int wx, int wy, int width, int height);
void MakeMachBand(int **img, int W, int H);
void DrawHistogram(int* histo, int width, int height);
void ComputeHistogram(int** IN, int* histo, int width, int height);
void HistogramEqualization(int** orgimg, int** outimg, int* histo, int width, int height);