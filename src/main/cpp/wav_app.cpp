#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <stdarg.h>

typedef struct  WAV_HEADER{
  char                RIFF[4];        //(4 byte chunkId) RIFF Header      Magic header
  unsigned long       ChunkSize;      //(4 byte chunkSize) RIFF Chunk Size  (unsigned long %lu)  (long %ld)
  char                WAVE[4];        //(4 byte format) WAVE Header
  char                fmt[4];         //(4 byte subchunk1Id) FMT header
  unsigned long       Subchunk1Size;  //(4 byte subchunk1Size) Size of the fmt chunk
  unsigned short      AudioFormat;    //(2 byte audioFormat) Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
  unsigned short      NumOfChan;      //(2 byte numChannels) Number of channels 1=Mono 2=Sterio
  unsigned long       SamplesPerSec;  //(4 byte sampleRate) Sampling Frequency in Hz
  unsigned long       bytesPerSec;    //(4 byte byteRate) bytes per second
  unsigned short      blockAlign;     //(2 byte blockAlign) 2=16-bit mono, 4=16-bit stereo
  unsigned short      bitsPerSample;  //(2 byte bitsPerSample) Number of bits per sample
  char                Subchunk2ID[4]; //(4 byte subchunk2Id) "data"  string
  unsigned long       Subchunk2Size;  //(4 byte subchunk2Size) Sampled data length
}wav_hdr;

char ans;

// Function prototypes
int getFileSize(FILE *inFile);
void flushKeyBoard(void);
// _iobuf myiobuf;

int main(int argc,char *argv[]){
    wav_hdr  wavHeader;
    
    FILE *wavFile;
    int headerSize = sizeof(wav_hdr), filelength = 0;
    
    _setmode( _fileno( stdin ), _O_BINARY ); //Set "stdin" to have binary mode:

    do{
      char input[10] = "";
      char path[40] = "";
      char filePath[40] = "";
      // char *input = NULL;
      // char *path = NULL;
      // char *filePath = NULL;
      ans = NULL;

      // fflush(stdin);
      // _flushall();
      
      printf("Pick wav file name: ");
      // gets(input);
      scanf("%s", &input);
      // scanf("%[^\n]",&input);
      
      sprintf(path, "C:\\Users\\Denis\\Music\\wav\\%s.wav", input);
      printf("path: %s\n", path);
      strcpy(filePath, path);
      // filePath = path;

      wavFile = fopen( filePath , "rb" );

      if(wavFile == NULL){
        printf("Can not able to open wave file\n");
        getch();
        exit(EXIT_FAILURE);
      }

      fread(&wavHeader, headerSize, 1, wavFile);
      filelength = getFileSize(wavFile);
            
      printf("headerSize                 :%d byte \n", headerSize);     
      printf("File is                    :%d byte \n", filelength);        
      printf("File is                    :%d Kbyte\n", filelength/1024);        
      printf("RIFF                       :%-4.4s \n" , wavHeader.RIFF);
      printf("WAVE                       :%-4.4s \n" , wavHeader.WAVE);
      printf("FMT                        :%-4.4s \n" , wavHeader.fmt);
      printf("Data size                  :%lu    \n" , wavHeader.ChunkSize);
      printf("Sampling Rate              :%lu    \n" , wavHeader.SamplesPerSec);
      printf("Number of bits used        :%d     \n" , wavHeader.bitsPerSample);
      printf("Number of channels         :%d     \n" , wavHeader.NumOfChan);
      printf("Number of bytes per second :%lu    \n" , wavHeader.bytesPerSec);
      printf("Data length                :%lu    \n" , wavHeader.Subchunk2Size);
      printf("Audio Format               :%d     \n" , wavHeader.AudioFormat);     
      printf("Block align                :%d     \n" , wavHeader.blockAlign);        
      printf("Data string                :%-4.4s\n\n", wavHeader.Subchunk2ID);
      
      fseek(wavFile, headerSize, SEEK_SET);
      
      int datCaunter = 0;
      
      short *data;
      data = (short *) calloc((wavHeader.ChunkSize/2-22), (wavHeader.bitsPerSample/8));//Функция calloc() возвращает указатель на первый байт выделенной области
      //data = (short *) malloc((wavHeader.ChunkSize/2-22)* sizeof(short)); // то же самое вид сбоку
      //void * malloc(size_t size);           // динамическое выделение памяти под символьную строку
      //void* realloc(void* ptr, size_t size) //перевыделение памяти по указамеля ранее выделенной
      
      if(!data) {
        printf ("Allocation failure.");
        exit (1);
      }

      for(int i=0 ; i<(wavHeader.ChunkSize/2-22) ; i++){
        fread(&data[datCaunter], sizeof(data[datCaunter]), 1, wavFile);
        datCaunter++;
      }
      
      for(datCaunter = 0; datCaunter < (wavHeader.ChunkSize/2-22)/4; datCaunter++){
        printf("data[%4d] :%5hd\t data[%4d] :%5hd\t data[%4d] :%5hd\t data[%4d] :%5hd\n",  
          datCaunter, data[datCaunter],
          datCaunter + ((wavHeader.ChunkSize/2-22)/4),   data[datCaunter + ((wavHeader.ChunkSize/2-22)/4)],
          datCaunter + 2*((wavHeader.ChunkSize/2-22)/4), data[datCaunter + 2*((wavHeader.ChunkSize/2-22)/4)],
          datCaunter + 3*((wavHeader.ChunkSize/2-22)/4), data[datCaunter + 3*((wavHeader.ChunkSize/2-22)/4)]);
      }
      fflush(wavFile);
      fclose(wavFile);

      printf("Try something else? (y/n)\n");

      // fflush(stdin);
      // _flushall();
      flushKeyBoard();  //это е**ная заплатка, потому что где то в буфер пеопадает лишний символ

      

      ans = getchar();
      
    }while(ans == 'y');

  printf("OUT");        
  getch();
  return(0);
}

// find the file size
int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}


void flushKeyBoard(void)
{
    int ch; //variable to read data into
    while((ch = getc(stdin)) != EOF && ch != '\n'){
      putchar(ch);
    }
}