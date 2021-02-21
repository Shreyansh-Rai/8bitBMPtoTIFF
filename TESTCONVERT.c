#include <stdio.h>
#include<stdlib.h>
 
void read_source_image();
void def_tiff();
void write_to_dest_image(int *data);
 
typedef unsigned short int usi;
typedef unsigned int uint;
 
struct BMP_HEADER
{
    char ch[2];
    uint szofbmp;
    usi dump1;
    usi dump2;
    uint offset;
};
struct BITMAP_INFO_HEADER
{
    uint size;
    int h;
    int w;
    usi colorpanes;
    usi bitsperpixel;
    int compression;
    int imagesize;
    int garbage[4];
};
 
struct TIFF_HEADER
{
  char ch_tiff[2];
  usi arb;
  uint offset_tiff_0;
};
 
struct TIFFTAG
{
  usi size;
  usi tagid[200];
  short datatype[200];
  long datacount[200];
  long dataoffset[200];
  int nextoff;
};
 
struct IMAGE_DATA
{ usi w ;
usi h ;
usi bps;
usi comp;
usi pi ;
uint off;
usi sppixel ;
usi rps ;
uint sbc ;
unsigned long int xr;
unsigned long int yr;
usi resunit ;
};
 
struct BMP_HEADER bmphead;
struct BITMAP_INFO_HEADER bih;
 
int color_table[256];
int pixeldata[512][512];
 
FILE *fp;
 
int main()
{
  read_source_image();
  def_tiff();
  write_to_dest_image(&pixeldata[0][0]);
 
  return 0;
}
 
 
void read_source_image()
{
struct BMP_HEADER bmphead;
struct BITMAP_INFO_HEADER bih;
 
    FILE *fp= fopen("barbara_gray.bmp","rb");
    fread(bmphead.ch,2,1,fp);
    fread(&bmphead.szofbmp,12,1,fp);
    fread(&bih.size,sizeof(bih),1,fp);
    fread(color_table,sizeof(color_table),1,fp);
 
 
        fseek(fp,bmphead.offset,SEEK_SET);
        for(int i=bih.h-1;i>0;i--)
        {
            for(int j=0;j<bih.w;j++)
            {
                fread(&pixeldata[i][j],1,1,fp);
            }
        }
        fclose(fp);
        return;
}
 
void def_tiff()
{
  struct TIFF_HEADER header;
  struct TIFFTAG directory;
  struct IMAGE_DATA imgdata;
 
  header.ch_tiff[0]='I';
  header.ch_tiff[1]='I';
  header.arb=42;
  header.offset_tiff_0=8;//
 
  directory.size=19;
 
  directory.tagid[0]=256; directory.dataoffset[0]=512;
  directory.tagid[1]=257; directory.dataoffset[1]=512;
  directory.tagid[2]=258; directory.dataoffset[2]=8;
  directory.tagid[3]=259; directory.dataoffset[3]=1;
  directory.tagid[4]=262; directory.dataoffset[4]=1;
  directory.tagid[5]=266; directory.dataoffset[5]=1;
  directory.tagid[6]=273; directory.dataoffset[6]=3266;
  directory.tagid[7]=274; directory.dataoffset[7]=1;
  directory.tagid[8]=277; directory.dataoffset[8]=1;
  directory.tagid[9]=278; directory.dataoffset[9]=512;
  directory.tagid[10]=279; directory.dataoffset[10]=262144;
  directory.tagid[11]=282; directory.dataoffset[11]=242;
  directory.tagid[12]=283; directory.dataoffset[12]=250;
  directory.tagid[13]=284; directory.dataoffset[13]=1;
  directory.tagid[14]=296; directory.dataoffset[14]=3;
  directory.tagid[15]=297; directory.dataoffset[15]=65536;
  directory.tagid[16]=318; directory.dataoffset[16]=258;
  directory.tagid[17]=319; directory.dataoffset[17]=274;
  directory.tagid[18]=700; directory.dataoffset[18]=322;
 
  for (int i=0;i<directory.size;i++)
  {
    directory.datatype[i]=3;
    directory.datacount[i]=1;
  }
  directory.datatype[6]=4; directory.datatype[10]=4;
  directory.datatype[11]=5; directory.datatype[12]=5;
  directory.datatype[16]=5; directory.datatype[17]=5;
  directory.datatype[18]=1; directory.datacount[15]=2;
  directory.datacount[16]=1; directory.datacount[17]=6;
  directory.datacount[18]=2943;
 
  imgdata.w=512;
  imgdata.h=512;
  imgdata.bps=8;
  imgdata.comp=1;
  imgdata.pi=1;
  imgdata.off=3266;
  imgdata.sppixel=1;
  imgdata.rps=512;
  imgdata.sbc=262144;
  imgdata.xr=242;
  imgdata.yr=250;
  imgdata.resunit=3;
 
  return;
}
 
void write_to_dest_image(int *data)
{
  struct TIFF_HEADER header;
  struct TIFFTAG directory;
  struct IMAGE_DATA imgdata;
 
 
  FILE *d = fopen("destination_image.tiff","wb");
  fwrite(header.ch_tiff,2,1,d);
  fwrite(&header.arb,6,1,d);
  fseek(d,header.offset_tiff_0,SEEK_SET);
  fwrite(&directory.size,2,1,d);
 
  for (int i=0;i<directory.size;i++)
  {
                fwrite(&directory.tagid[i],2,1,d);
    fwrite(&directory.datatype[i],2,1,d);
    fwrite(&directory.datacount[i],4,1,d);
    fwrite(&directory.dataoffset[i],4,1,d);
  }
 
  fwrite(&directory.nextoff,4,1,d);
  fseek(d,imgdata.off,SEEK_SET);
 
  for (int i=0 ;i<imgdata.h;i++)
  {
    for (int j=0;j<imgdata.w;j++)
    {
      fputc(*((data+(i*imgdata.h))+j),d);
    }
  }
  fclose(d);
  return;
}

