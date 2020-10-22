/* 
 * pageio.c --- saving and loading crawler webpage files
 * 
 * Author: 
 * Created: 
 * Version: 1.0
 * 
 * Description: pagesave saves an existing webpage to a file with a
 * numbered name (e.g. 1,2,3 etc); pageload creates a new page by
 * loading a numbered file. For pagesave, the directory must exist and
 * be writable; for loadpage it must be readable.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "webpage.h"
#include "queue.h"
#include "hash.h"
#include <sys/stat.h>

/*
 * pagesave -- save the page in filename id in directory dirnm
 *
 * returns: 0 for success; nonzero otherwise
 *
 * The suggested format for the file is:
 *   <url>
 *   <depth>
 *   <html-length>
 *   <html>
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	struct stat st= {0};
	char path[260];
	sprintf(path,"../%s",dirname);

	if (stat(path,&st)==-1){ //if unable to get file properties 
		if (mkdir(path,0700)!=-1){ //make dir
			sprintf(path,"%s/%d",path,id); //now the path to the specific file
			
			FILE *fp = fopen(path,"w");//make file 
			if (access(path,W_OK)!=0)//checks users permission
				return -1;//file does not exist
			fprintf(fp,"%s\n",webpage_getURL(pagep));
			fprintf(fp,"%d\n",webpage_getDepth(pagep));
			fprintf(fp,"%d\n",webpage_getHTMLlen(pagep));
			fprintf(fp,"%s\n",webpage_getHTML(pagep));
		fclose(fp);
		}
	} else {
			sprintf(path,"%s/%d",path,id); //now the path to the specific file
			
			FILE *fp = fopen(path,"w");//make file 
			if (access(path,W_OK)!=0)//checks users permission
				return -1;//file does not exist
			fprintf(fp,"%s\n",webpage_getURL(pagep));
			fprintf(fp,"%d\n",webpage_getDepth(pagep));
			fprintf(fp,"%d\n",webpage_getHTMLlen(pagep));
			fprintf(fp,"%s\n",webpage_getHTML(pagep));
			fclose(fp);
	}
	
	return 0;
}
/* 
 * pageload -- loads the numbered filename <id> in directory <dirnm>
 * into a new webpage
 *
 * returns: non-NULL for success; NULL otherwise
 */
webpage_t *pageload(int id, char *dirnm) {
	char path [260];
	sprintf(path,"../%s/%d",dirnm,id);
	FILE *fp=fopen(path,"r");

	// gets the URL
  char url[100];
	fscanf(fp,"%[^\n]",url);
	//	printf("url=%s\n",url);
	
	// gets the depth
	//char dchar[2];
	int depth;
	fscanf(fp,"%d",&depth);
	//int depth = atoi(dchar);
	//	printf("dchar=%d\n",dchar);

	webpage_t *newpage=webpage_new(url,depth,NULL);
	webpage_fetch(newpage);
	//	printf("html=%s\n",webpage_getHTML(newpage));

  fclose(fp);
	return newpage;
}
