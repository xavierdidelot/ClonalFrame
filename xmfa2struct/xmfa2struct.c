/***************************************************************************
 *   Copyright (C) 2007 by Xavier Didelot                                  *
 *   didelot@stats.ox.ac.uk                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#define UNLINKED 9
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char
convert (char in)
{
  switch (in)
    {
    case 'a':
      return '0';
    case 'A':
      return '0';
    case 't':
      return '1';
    case 'T':
      return '1';
    case 'c':
      return '2';
    case 'C':
      return '2';
    case 'g':
      return '3';
    case 'G':
      return '3';
    default:
      return 'N';
    }
}

void
Fgets (char *buf, int l, FILE * f)
{
  fgets (buf, l, f);
  while (strncmp (buf, "#", 1) == 0)
    fgets (buf, l, f);
  if (buf[strlen (buf) - 1] == '\n')
    buf[strlen (buf) - 1] = '\0';
  if (buf[strlen (buf) - 1] == '\r')
    buf[strlen (buf) - 1] = '\0';
}

void
extractName (char *buf, char *buf2)
{
  if (buf[0] != '>')
    {
      buf2[0] = '\0';
      return;
    }
  int i = 1;
  while (buf[i] == ' ')
    i++;
  strcpy (buf2, buf + i);
  if (strstr (buf2, ":") != NULL)
    (strstr (buf2, ":"))[0] = '\0';
}


int
main (int argc, char **argv)
{
  if (argc != 3)
    {
      printf ("usage: %s xmfa_file structure_file\n", argv[0]);
      return (0);
    }
  char *filename = argv[1];
  printf ("Reading xmfa file...\n");
  char *names[10000];
  char buf[10000];
  char buf2[1000];
  //First count the number of genomes
  FILE *f = fopen (filename, "r");
  if (f == NULL)
    {
      printf ("Unable to open input file\n");
      abort ();
    }
  int nbgenomes = 0;
  int i, l, j;
  int deb = 0;
  while (!feof (f))
    {
      Fgets (buf, 10000, f);
      if (buf[0] == '>')
	{
	  names[nbgenomes++] = (char *) calloc (1000, sizeof (char));
	  extractName (buf, names[nbgenomes - 1]);
	}
      if (buf[0] == '=' || feof (f))
	break;
    }

  //Then count the number of LCBs where all sequences are present
  rewind (f);
  int *check = (int *) calloc (nbgenomes, sizeof (int));
  int lcb = 0;
  while (!feof (f))
    {
      Fgets (buf, 10000, f);
      if (buf[0] == '>')
	{
	  extractName (buf, buf2);
	  for (int i = 0; i < nbgenomes; i++)
	    if (strcmp (buf2, names[i]) == 0)
	      {
		check[i] = 1;
		break;
	      }
	}
      if (strncmp (buf, "=", 1) == 0 || feof (f))
	{
	  int sum = 0;
	  for (int i = 0; i < nbgenomes; i++)
	    sum += check[i];
	  if (sum == nbgenomes)
	    {
	      lcb++;
	      for (int i = 0; i < nbgenomes; i++)
		check[i] = 0;
	    }
	  else
	    {
	      int sum = 0;
	      for (int i = 0; i < nbgenomes; i++)
		sum += check[i];
	      if (sum > 0)
		{
		  int missing = 0;
		  for (int i = 0; i < nbgenomes; i++)
		    if (check[i] == 0)
		      {
			missing = i;
			break;
		      }
		  printf
		    ("Warning: fragment %d is incomplete (does not contain strain %s)\nWarning: ClonalFrame is running only on the first %d fragments\n",
		     lcb + 1, names[missing], lcb);
		}
	      break;
	    }
	}
    }
  free (check);
  //Then count the length of the alignment
  rewind (f);
  int length = 0;
  for (l = 0; l < lcb; l++)
    {
      while (1)
	{
	  Fgets (buf, 10000, f);
	  if (buf[0] == '>')
	    {
	      extractName (buf, buf2);
	      if (strcmp (buf2, names[0]) == 0)
		break;
	    }
	}
      Fgets (buf, 10000, f);
      while (buf[0] != '>' && buf[0] != '=' && !feof (f))
	{
	  length += strlen (buf);
	  Fgets (buf, 10000, f);
	}
    }
  printf ("Found %d strains, %d blocks and %d sites.\n", nbgenomes, lcb,
	  length);
  fclose (f);

  //Then read the data
  char **data = (char **) calloc (nbgenomes, sizeof (char *));
  for (int i = 0; i < nbgenomes; i++)
    data[i] = (char *) calloc (length + lcb - 1, sizeof (char));
  FILE **in = (FILE **) calloc (nbgenomes, sizeof (FILE *));
  char **bufin = (char **) calloc (nbgenomes, sizeof (char *));
  int *pos = (int *) calloc (nbgenomes, sizeof (int));
  for (i = 0; i < nbgenomes; i++)
    {
      in[i] = fopen (filename, "r");
      bufin[i] = (char *) calloc (10000, sizeof (char));
    }
  for (l = 0; l < lcb; l++)
    {				//For each LCB
      for (i = 0; i < nbgenomes; i++)
	{			//For each genome
	  //Find next bit of alignment
	  extractName (bufin[i], buf2);
	  while (strcmp (names[i], buf2) != 0)
	    {
	      Fgets (bufin[i], 10000, in[i]);
	      extractName (bufin[i], buf2);
	    }
	  if (i == 0)
	    {
	      if (strstr (bufin[i], ":") != NULL)
		sscanf (strstr (bufin[i], ":") + 1, "%d", &deb);
	      else
		deb++;
	    }
	  Fgets (bufin[i], 10000, in[i]);
	  //And copy it
	  while (bufin[i][0] != '>' && bufin[i][0] != '=' && !feof (in[i]))
	    {
	      for (j = 0; j < strlen (bufin[i]); j++)
		data[i][pos[i]++] = convert (bufin[i][j]);
	      Fgets (bufin[i], 10000, in[i]);
	    }
	  //Add the unlinked symbol if there are more LCB coming
	  if (l < lcb - 1)
	    data[i][pos[i]++] = UNLINKED;
	}
    }
  for (i = 0; i < nbgenomes; i++)
    if (pos[i] != length + lcb - 1)
      {
	printf ("Invalid input file: the sequence for %s is incomplete.\n",
		names[i]);
	abort ();
      }

  //Clear the memory
  for (i = 0; i < nbgenomes; i++)
    {
      fclose (in[i]);
      free (bufin[i]);
    }
  free (in);
  free (pos);
  free (bufin);
//Find the sites to print
  printf ("Writing structure file...\n");
  int *polys = (int *) calloc (length + lcb - 1, sizeof (int));
  int p = 0;
  for (i = 0; i < length + lcb - 1; i++)
    {
      int r=0;
      while (data[r][i] == 'N' && r<nbgenomes) r++;
      bool poly = false;
      for (j = 0; j < nbgenomes; j++)
	if (data[r][i] != data[j][i] && data[r][i] != 'N' && data[j][i] != 'N')
	  poly = true;
      if (i == 0 || i == length + lcb - 2 || data[0][i - 1] == UNLINKED
	  || data[0][i + 1] == UNLINKED || poly)
	polys[p++] = i;
    }
//Then write the map
  FILE *out = fopen (argv[2], "w");
  for (i = 0; i < p; i++)
    if (i == 0 || data[0][polys[i] - 1] == UNLINKED)
      fprintf (out, "-1 ");
    else
      fprintf (out, "%d ", polys[i] - polys[i - 1]);
  fprintf (out, "\n");
//Then write the rest of the file
  bool missing = false;
  for (j = 0; j < nbgenomes; j++)
    {fprintf(out,"%s ",names[j]);
      for (i = 0; i < p; i++)
	if (data[j][polys[i]] != 'N')
	  fprintf (out, "%c ", data[j][polys[i]]);
	else
	  {
	    if (missing == false)
	      printf ("Found missing data, encoding it as -9.\n");
	    fprintf (out, "-9 ");
	    missing = true;
	  };
      fprintf (out, "\n");
    }
  fclose (out);
//Clear memory
  for (i = 0; i < nbgenomes; i++)
    free (names[i]);
  free (polys);
  for (i = 0; i < nbgenomes; i++)
    free (data[i]);
  free (data);
printf("All done.\n");
}
