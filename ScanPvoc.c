// ScanPvoc.c
//
// by Christopher Saunders
// topher6345@gmail.com
// for the Audio Programming Class
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXRAND 30

int main(int argc, char *argv[]) {
  FILE *fp;
  time_t curtime;
  struct tm *loctime;
  char filename[1024];
  char pvxname[1024];
  char aifname[1024];
  char remove[1024] = "rm ";
  char buffer[512];
  int stringsize;
  unsigned int i, max;
  if (argc < 2) {
    printf("\n Usage: ScanPvoc filename \n\n");
    return 1;
  }

  char pvxcreate[128] = "pvanal -s 44100 ";

  srand(time(NULL));

  strcpy(filename, argv[1]);
  strcat(filename, " ");
  strcat(pvxcreate, filename);

  // stringsize = strlen(argv[1]);
  // strncpy(pvxname, argv[1], stringsize - 4);
  strcpy(pvxname, "toss.pvx");

  strcat(pvxcreate, pvxname);
  system(pvxcreate);
  printf("\n %s \n", pvxcreate);

  fp = NULL;
  fp = fopen("toss.csd", "w");
  if (fp == NULL) {
    printf("WARNING: unable to create file %s\n", argv[1]);
    perror("");
  }
  fprintf(fp, "<CsoundSynthesizer> \n");
  fprintf(fp, "<CsInstruments>\n");
  fprintf(fp, "sr=44100\n");
  fprintf(fp, "ksmps=10\n");
  fprintf(fp, "nchnls=2\n");
  fprintf(fp, "0dbfs=1\n");
  fprintf(fp, "#define FILE #\"%s\"#\n", pvxname);

  fprintf(fp, "git1 ftgen 1, 0, 128, 10,1");

  max = (unsigned int)rand() % MAXRAND;
  for (i = 0; i < max; i++) {
    fprintf(fp, ",1");
  }

  fprintf(fp, "\n");
  fprintf(fp, "git11 ftgen 11, 0, 128, 10, 1\n");
  fprintf(fp, "git2 ftgen 2, 0, 128, -7,  1, 128,  .5\n");
  fprintf(fp, "git3 ftgen 3, 0, 128, -7, 0, 128, 1\n");
  fprintf(fp, "git4 ftgen 4, 0, 128, -7, 1, 128, 1\n");
  fprintf(fp, "git5 ftgen 5, 0, 128, -7, 0, 128, 0\n");
  fprintf(fp, "instr cntrl\n");
  fprintf(fp, "Sfile = $FILE.\n");
  fprintf(fp, "gilen filelen Sfile\n");
  fprintf(fp, "print gilen\n");
  fprintf(fp, "event_i \"i\", \"scanpvoc\", 0, gilen\n");
  fprintf(fp, "endin\n");
  fprintf(fp, "instr scanpvoc\n");
  fprintf(fp, "ipos = 1\n");
  fprintf(fp, "imass = 2\n");
  fprintf(fp, "istiff = 3\n");
  fprintf(fp, "istiff2 = 33\n");
  fprintf(fp, "idamp = 4\n");
  fprintf(fp, "ivel = 5\n");
  fprintf(fp, "a0 scantable 1, 440, ipos, imass, istiff, idamp, ivel\n");
  fprintf(fp, "kphase phasor 10\n");
  fprintf(fp, "klfo tablei kphase, 1\n");
  fprintf(fp, "chnset kphase, \"lfo\"\n");
  fprintf(fp, "ktime = ((klfo*.5+.5) %% 1) * gilen+.001\n");
  fprintf(fp, "if ktime >0 then\n");
  fprintf(fp, "ares pvoc  ktime , 1, $FILE., 0\n");
  fprintf(fp, "outs ares,ares\n");
  fprintf(fp, "endif\n");
  fprintf(fp, "endin\n");
  fprintf(fp, "</CsInstruments>\n");
  fprintf(fp, "<CsScore>\n");
  fprintf(fp, "i 1 0 10\n");
  fprintf(fp, "</CsScore>\n");
  fprintf(fp, "</CsoundSynthesizer>\n");
  fclose(fp);

  stringsize = strlen(filename);
  strncpy(aifname, filename, stringsize - 4);
  curtime = time(NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 256, "_ScanPvoc_%m%d%I%M%S%p.aiff toss.csd", loctime);
  strcat(aifname, buffer);
  char flags[128] = " csound -f -d -A -o ";
  strcat(flags, aifname);

  system(flags);
  strcat(remove, pvxname);


  system("rm toss.csd");
  system("rm toss.pvx");

  return 0;
}
