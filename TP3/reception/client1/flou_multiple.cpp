
// flou_multiple.cpp : floute plusieurs fois une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"
#include <string.h>

int main(int argc, char* argv[])
{
char cNomImgLue[250], cNomImgEcrite[250];
int nH, nW, nTaille, S;

if (argc != 3) {
    printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
    exit (1) ;
}

sscanf (argv[1],"%s",cNomImgLue) ;
sscanf (argv[2],"%s",cNomImgEcrite);
   //   for (int i=0; i < nTaille; i++)
   // {
   //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
   //  }

//5 filtre_flou2
char commande[500];
sprintf(commande, "./filtre_flou2 %s ../image/imageFlouTampon.pgm", cNomImgLue);
system(commande);
system("./filtre_flou2 ../image/imageFlouTampon.pgm ../image/imageFlouTampon2.pgm");
system("./filtre_flou2 ../image/imageFlouTampon2.pgm ../image/imageFlouTampon3.pgm");
system("./filtre_flou2 ../image/imageFlouTampon3.pgm ../image/imageFlouTampon4.pgm");
system("./filtre_flou2 ../image/imageFlouTampon4.pgm ../image/imageFlouTampon5.pgm");
system("./filtre_flou2 ../image/imageFlouTampon5.pgm ../image/imageFlouFinale.pgm");





return 1;
}
